#pragma once
#include "Engine/Asset/AssetTypes/TextureAsset.hpp"
#include "Engine/Asset/AssetTypes/ModelAsset.hpp"
#include "Engine/Asset/AssetTypes/PixelShaderAsset.hpp"
#include "Engine/Asset/AssetTypes/VertexShaderAsset.hpp"
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