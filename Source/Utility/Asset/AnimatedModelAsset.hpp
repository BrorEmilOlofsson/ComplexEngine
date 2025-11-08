#pragma once
#include "Utility/Asset/Asset.hpp"
#include "Utility/Asset/AssetHandle.hpp"

namespace Simple
{
	class AnimatedModel;

	using AnimatedModelAsset = Asset<AnimatedModel>;
	using AnimatedModelAssetHandle = AssetHandle<AnimatedModel>;
}