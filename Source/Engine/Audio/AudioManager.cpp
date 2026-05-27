#include <Engine/Precompiled/EnginePch.hpp>
#include "AudioManager.hpp"
#include <External/fmod/studio/fmod_studio.hpp>
#include "FMODUtility.hpp"

namespace CLX
{
    [[nodiscard]] std::optional<FMOD::Sound*> LoadSoundFile(FMOD::System& system, const std::filesystem::path& path, bool enable3d, bool shouldStream, bool shouldLoop)
    {
        // We always load looping to avoid click at the end of the file
        // if it's toggled later.
        FMOD_MODE loadMode = FMOD_DEFAULT | FMOD_LOOP_NORMAL;
        loadMode |= enable3d ? FMOD_3D : FMOD_2D;
        loadMode |= shouldStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

        FMOD::Sound* ptr = nullptr;
        [[maybe_unused]] auto result = system.createSound((path.string()).c_str(), loadMode, nullptr, &ptr);
        ASSERT(result == FMOD_OK);
        if (ptr)
        {
            // Reset looping
            if (!shouldLoop)
            {
                result = ptr->setMode(FMOD_LOOP_OFF);
                ASSERT(result == FMOD_OK);
            }

            return ptr;
        }

        return std::nullopt;
    }

    AudioManager::AudioManager()
    {
        FMOD::Studio::System::create(&mFMODStudioSystem);
        [[maybe_unused]] auto result = mFMODStudioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
        ASSERT(result == FMOD_OK);
        result = mFMODStudioSystem->getCoreSystem(&mFMODSystem);
        ASSERT(result == FMOD_OK);
    }

    AudioChannelID AudioManager::Play(AudioAssetHandle handle, const float volume, const Vector3f& position)
    {
        ASSERT(handle);

        FMOD::Channel* soundChannel = nullptr;
        [[maybe_unused]] auto errorCode = mFMODSystem->playSound(handle->soundPtr, nullptr, true, &soundChannel);
        ASSERT(errorCode == FMOD_OK);

        if (soundChannel)
        {
            FMOD_MODE currentMode;
            handle->soundPtr->getMode(&currentMode);
            if (currentMode & FMOD_3D)
            {
                FMOD_VECTOR relativePos = ToFmodVector(position);
                soundChannel->set3DAttributes(&relativePos, nullptr);
            }

            soundChannel->setVolume(volume);
            soundChannel->setPaused(false);

            const AudioChannelID chId = AudioChannelID{ mNextChId++ };
            mChannels[chId] = soundChannel;
            return chId;
        }

        return InvalidID<AudioChannelID>();
    }

    void AudioManager::Update()
    {
        [[maybe_unused]] auto result = mFMODStudioSystem->update();
        ASSERT(result == FMOD_OK);
    }

    void AudioManager::SetChannelPosition(AudioChannelID channelId, const Vector3f& position)
    {
        auto channelIt = mChannels.find(channelId);
        if (channelIt == mChannels.end())
        {
            return;
        }

        FMOD_VECTOR relativePos = ToFmodVector(position);
        [[maybe_unused]] auto result = channelIt->second->set3DAttributes(&relativePos, nullptr);
        ASSERT(result == FMOD_OK);
    }

    void AudioManager::SetChannelVolume(AudioChannelID channelId, const float volume)
    {
        auto channelIt = mChannels.find(channelId);
        if (channelIt == mChannels.end())
        {
            return;
        }

        [[maybe_unused]] auto result = channelIt->second->setVolume(volume);
        ASSERT(result == FMOD_OK);
    }

    std::optional<Audio> AudioManager::LoadAudio(const std::filesystem::path& path)
    {
        if (auto sound = ::CLX::LoadSoundFile(*mFMODSystem, path, true, false, false))
        {
            mSounds[path] = *sound;
            return Audio{ *sound };
        }

        return std::nullopt;
    }

}