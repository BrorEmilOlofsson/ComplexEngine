#pragma once
#include "Engine/Asset/Asset.hpp"
#include "Engine/Asset/AssetHandle.hpp"

#include "Engine/Scene/Scene.hpp"

namespace CLX
{
    using SceneAsset = Asset<Scene>;
    using SceneAssetHandle = AssetHandle<Scene>;
}