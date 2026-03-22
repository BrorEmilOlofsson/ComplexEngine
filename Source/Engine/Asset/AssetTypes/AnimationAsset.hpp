#pragma once
#include "Engine/Asset/Asset.hpp"
#include "Engine/Asset/AssetHandle.hpp"

#include "Engine/Graphics/Animation/Animation.hpp"

namespace CLX
{
	using AnimationAsset = Asset<Animation>;
	using AnimationAssetHandle = AssetHandle<Animation>;
}