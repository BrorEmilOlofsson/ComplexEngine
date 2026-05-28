#pragma once
#include "Engine/Asset/AssetTypes/AudioAsset.hpp"
#include "Engine/Audio/AudioChannelID.hpp"


namespace CLX
{

    struct AudioComponent
    {
        AudioAssetHandle audioAsset;
        AudioChannelID channelId = InvalidID<AudioChannelID>();
        float volume = 1.0f;
    };

    void RegisterAudioComponent();
}