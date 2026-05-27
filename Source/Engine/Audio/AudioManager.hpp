#pragma once
#include <unordered_map>
#include <filesystem>
#include <optional>

#include <fmod/core/fmod.hpp>
#include <fmod/studio/fmod_studio.hpp>

#include "Engine/Math/Vector3.hpp"
#include "Engine/Utility/IDWrapper.hpp"
#include "Engine/Asset/AssetTypes/AudioAsset.hpp"

namespace CLX
{
    using AudioChannelID = IDWrapper<size_t, struct AudioChannelIDTag>;

    class AudioManager
    {
    public:

        AudioManager();

        AudioChannelID Play(AudioAssetHandle handle, const float volume = 1.0f, const Vector3f& position = Vector3f::Zero());

        void Update();

        void SetChannelPosition(AudioChannelID channelId, const Vector3f& position);
        void SetChannelVolume(AudioChannelID channelId, const float volume);

        std::optional<Audio> LoadAudio(const std::filesystem::path& path);

    private:

        FMOD::Studio::System* mFMODStudioSystem = nullptr;
        FMOD::System* mFMODSystem = nullptr;
        std::unordered_map<std::filesystem::path, FMOD::Sound*> mSounds;
        std::unordered_map<AudioChannelID, FMOD::Channel*> mChannels;
        size_t mNextChId = 1;

    };
}