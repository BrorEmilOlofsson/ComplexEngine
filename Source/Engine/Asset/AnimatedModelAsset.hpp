#pragma once
#include "Engine/Asset/Asset.hpp"
#include "Engine/Asset/AssetHandle.hpp"

namespace Simple
{
	class AnimatedModel;

	using AnimatedModelAsset = Asset<AnimatedModel>;
	using AnimatedModelAssetHandle = AssetHandle<AnimatedModel>;
}