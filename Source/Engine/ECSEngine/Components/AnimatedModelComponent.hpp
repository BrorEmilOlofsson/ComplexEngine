#pragma once
#include "Utility/Asset/AnimatedModelAsset.hpp"
#include "Graphics/Animation/AnimationPlayer.hpp"
#include "Utility/Asset/TextureAsset.hpp"

namespace Simple
{

	struct AnimatedModelComponent
	{
		AnimatedModelAssetHandle modelHandle;
		std::array<TextureAssetHandle, 3> textures;
		AnimationPlayer animationPlayer;
	};

	void RegisterAnimatedModelComponent();
}