#pragma once
#include "Utility/Math/Matrix4x4.hpp"
#include "Graphics/GraphicsConstants.hpp"
#include "Utility/Asset/AnimatedModelAsset.hpp"
#include "Utility/Asset/AnimationAsset.hpp"
#include "Utility/Asset/PixelShaderAsset.hpp"
#include "Utility/Asset/VertexShaderAsset.hpp"
#include "Graphics/Animation/AnimationPlayer.hpp"

namespace Simple
{

	struct AnimationComponent final
	{
		AnimationPlayer animationPlayer;
		std::array<Matrix4x4f, Global_Max_Joints> jointMatrices;
		AnimationAssetHandle animation;
		AnimatedModelAssetHandle animatedModel;
		PixelShaderAssetHandle pixelShader;
		VertexShaderAssetHandle vertexShader;
	};

	void RegisterAnimationComponent();
}