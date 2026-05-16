#pragma once
#include "Engine/Asset/AssetTypes/AnimatedModelAsset.hpp"
#include "Engine/Graphics/Animation/AnimationPlayer.hpp"
#include "Engine/Asset/AssetTypes/TextureAsset.hpp"

namespace CLX
{

	struct AnimatedModelComponent
	{
		AnimatedModelAssetHandle modelHandle;
        AnimationAssetHandle defaultAnimation;
		std::array<TextureAssetHandle, 3> textures;
		AnimationPlayer animationPlayer;
	};

	void RegisterAnimatedModelComponent();
}