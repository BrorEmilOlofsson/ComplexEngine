#pragma once
#include "Engine/Asset/AssetTypes/AudioAsset.hpp"


namespace CLX
{

    struct AudioComponent
    {
        AudioAssetHandle audioAsset;
        float volume = 1.0f;
    };

    void RegisterAudioComponent();
}