#pragma once
#include "Engine/Math/Transform3.hpp"
#include "Engine/Asset/AssetTypes/TextureAsset.hpp"
#include "Engine/Asset/AssetTypes/MeshAsset.hpp"
#include "Engine/Asset/AssetTypes/PixelShaderAsset.hpp"
#include "Engine/Asset/AssetTypes/VertexShaderAsset.hpp"

namespace CLX
{
	
	class SkyBox final
	{
	public:
		MeshAssetHandle mesh;
		Transform transform;
		TextureAssetHandle texture;
		PixelShaderAssetHandle pixelShader;
		VertexShaderAssetHandle vertexShader;
		bool useSkyBoxFlooring = false;
		bool useUnityShader = false;
	};
}