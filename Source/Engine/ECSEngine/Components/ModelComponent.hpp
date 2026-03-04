#pragma once
#include "Engine/Asset/TextureAsset.hpp"
#include "Engine/Asset/ModelAsset.hpp"
#include "Engine/Asset/PixelShaderAsset.hpp"
#include "Engine/Asset/VertexShaderAsset.hpp"
#include "Engine/Utility/Blackboard.hpp"
#include <array>

namespace CLX
{
	struct ModelComponent final
	{
		std::array<TextureAssetHandle, 3> textures;
		ModelAssetHandle model;
		PixelShaderAssetHandle pixelShader;
		VertexShaderAssetHandle vertexShader;
	};

	void Initialize(ModelComponent& component, const Blackboard& blackboard);
	void RegisterModelComponent();
}