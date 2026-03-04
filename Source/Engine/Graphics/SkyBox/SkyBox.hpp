#pragma once
#include "Engine/Math/Transform3.hpp"
#include "Engine/Asset/TextureAsset.hpp"
#include "Engine/Asset/MeshAsset.hpp"
#include "Engine/Asset/PixelShaderAsset.hpp"
#include "Engine/Asset/VertexShaderAsset.hpp"

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