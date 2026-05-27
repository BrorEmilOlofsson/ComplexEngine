#pragma once
#include "Engine/Asset/Asset.hpp"
#include "Engine/Asset/AssetHandle.hpp"
#include "Engine/Audio/Audio.hpp"

namespace CLX
{
    using AudioAsset = Asset<Audio>;
    using AudioAssetHandle = AssetHandle<Audio>;
}