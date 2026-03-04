#pragma once
#include "Engine/Asset/AnimatedModelAsset.hpp"
#include "Engine/Graphics/Animation/AnimationPlayer.hpp"
#include "Engine/Asset/TextureAsset.hpp"

namespace CLX
{

	struct AnimatedModelComponent
	{
		AnimatedModelAssetHandle modelHandle;
		std::array<TextureAssetHandle, 3> textures;
		AnimationPlayer animationPlayer;
	};

	void RegisterAnimatedModelComponent();
}