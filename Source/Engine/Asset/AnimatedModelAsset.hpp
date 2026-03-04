#pragma once
#include "Engine/Asset/Asset.hpp"
#include "Engine/Asset/AssetHandle.hpp"

namespace CLX
{
	class AnimatedModel;

	using AnimatedModelAsset = Asset<AnimatedModel>;
	using AnimatedModelAssetHandle = AssetHandle<AnimatedModel>;
}