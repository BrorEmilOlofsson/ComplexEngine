#pragma once
#include "Utility/Asset/TextureAsset.hpp"
#include "Utility/Asset/ModelAsset.hpp"
#include "Utility/Asset/PixelShaderAsset.hpp"
#include "Utility/Asset/VertexShaderAsset.hpp"
#include "Utility/Blackboard.hpp"
#include <array>

namespace Simple
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