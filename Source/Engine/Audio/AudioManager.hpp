#pragma once
#include <unordered_map>
#include <filesystem>
#include <optional>

#include <fmod/core/fmod.hpp>
#include <fmod/studio/fmod_studio.hpp>

#include "Engine/Math/Point3.hpp"
#include "Engine/Asset/AssetTypes/AudioAsset.hpp"
#include "Engine/Audio/AudioChannelID.hpp"

namespace CLX
{
    class AudioManager
    {
    public:

        AudioManager();

        AudioChannelID Play(AudioAssetHandle handle, float volume = 1.0f, const Point3f& position = Point3f::Zero());

        void Update();

        void SetChannelPosition(AudioChannelID channelId, const Point3f& position);
        void SetChannelVolume(AudioChannelID channelId, float volume);
        void SetChannelPaused(AudioChannelID channelId, bool paused);

        std::optional<Audio> LoadAudio(const std::filesystem::path& path);

    private:

        FMOD::Studio::System* mFMODStudioSystem = nullptr;
        FMOD::System* mFMODSystem = nullptr;
        std::unordered_map<std::filesystem::path, FMOD::Sound*> mSounds;
        std::unordered_map<AudioChannelID, FMOD::Channel*> mChannels;
        uint32_t mNextChannelID = 0;
    };
}