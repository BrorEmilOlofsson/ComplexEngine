#pragma once
#include "Engine/Asset/Asset.hpp"
#include "Engine/Asset/AssetHandle.hpp"

namespace CLX
{

    class Scene;

    using SceneAsset = Asset<Scene>;
    using SceneAssetHandle = AssetHandle<Scene>;
}