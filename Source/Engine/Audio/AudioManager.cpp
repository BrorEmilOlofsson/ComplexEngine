#include <Engine/Precompiled/EnginePch.hpp>
#include "AudioManager.hpp"
#include <External/fmod/studio/fmod_studio.hpp>
#include "FMODUtility.hpp"
#include "Engine/Utility/Assert.hpp"

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
        ASSERT(result == FMOD_OK, "Failed to create sound");
        if (ptr)
        {
            // Reset looping
            if (!shouldLoop)
            {
                result = ptr->setMode(FMOD_LOOP_OFF);
                ASSERT(result == FMOD_OK, "Failed to set sound mode");
            }

            return ptr;
        }

        return std::nullopt;
    }

    AudioManager::AudioManager(FMOD::Studio::System* studioSystem, FMOD::System* coreSystem)
        : mFMODStudioSystem(studioSystem)
        , mFMODSystem(coreSystem)
    {
    }

    AudioManager AudioManager::Create()
    {
        FMOD::Studio::System* studioSystem = nullptr;
        FMOD::System* coreSystem = nullptr;
        [[maybe_unused]] auto result = FMOD::Studio::System::create(&studioSystem);
        ASSERT(result == FMOD_OK, "Failed to create FMOD Studio system");
        result = studioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
        ASSERT(result == FMOD_OK, "Failed to initialize FMOD Studio system");
        result = studioSystem->getCoreSystem(&coreSystem);
        ASSERT(result == FMOD_OK, "Failed to get FMOD core system");
        return AudioManager(studioSystem, coreSystem);
    }

    AudioChannelID AudioManager::Play(AudioAssetHandle handle, const float volume, const Point3f& position)
    {
        ASSERT(handle, "Invalid audio asset handle");

        FMOD::Channel* soundChannel = nullptr;
        [[maybe_unused]] auto errorCode = mFMODSystem->playSound(handle->soundPtr, nullptr, true, &soundChannel);
        ASSERT(errorCode == FMOD_OK, "Failed to play sound");

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

            const AudioChannelID chId = AudioChannelID{ mNextChannelID++ };
            mChannels[chId] = soundChannel;
            return chId;
        }

        return InvalidID<AudioChannelID>();
    }

    void AudioManager::Update()
    {
        [[maybe_unused]] auto result = mFMODStudioSystem->update();
        ASSERT(result == FMOD_OK, "Failed to update FMOD Studio system");
    }

    void AudioManager::SetChannelPosition(AudioChannelID channelId, const Point3f& position)
    {
        auto channelIt = mChannels.find(channelId);
        if (channelIt == mChannels.end())
        {
            return;
        }

        FMOD_VECTOR relativePos = ToFmodVector(position);
        [[maybe_unused]] auto result = channelIt->second->set3DAttributes(&relativePos, nullptr);
        ASSERT(result == FMOD_OK, "Failed to set channel 3D attributes");
    }

    void AudioManager::SetChannelVolume(AudioChannelID channelId, const float volume)
    {
        auto channelIt = mChannels.find(channelId);
        if (channelIt == mChannels.end())
        {
            return;
        }

        [[maybe_unused]] auto result = channelIt->second->setVolume(volume);
        ASSERT(result == FMOD_OK, "Failed to set channel volume");
    }

    void AudioManager::SetChannelPaused(AudioChannelID channelId, bool paused)
    {
        auto channelIt = mChannels.find(channelId);
        if (channelIt == mChannels.end())
        {
            return;
        }
        [[maybe_unused]] auto result = channelIt->second->setPaused(paused);
        ASSERT(result == FMOD_OK, "Failed to set channel paused state");
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