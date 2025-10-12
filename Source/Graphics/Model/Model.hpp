#pragma once
#include <array>
#include "Utility/Asset/TextureAsset.hpp"
#include "Utility/Asset/MeshAsset.hpp"
#include "Utility/Asset/PixelShaderAsset.hpp"
#include "Utility/Asset/VertexShaderAsset.hpp"

namespace Simple
{
	struct Model final
	{
		std::array<TextureAssetHandle, 3> textures;
		MeshAssetHandle mesh;
		PixelShaderAssetHandle pixelShader;
		VertexShaderAssetHandle vertexShader;
	};
}