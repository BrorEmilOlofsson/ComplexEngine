#pragma once
#include "Engine/Math/Transform3.hpp"
#include "Engine/Asset/TextureAsset.hpp"
#include "Engine/Asset/MeshAsset.hpp"
#include "Engine/Asset/PixelShaderAsset.hpp"
#include "Engine/Asset/VertexShaderAsset.hpp"

namespace CLX
{

	struct SkyBoxComponent final
	{
		Transform transform;
		MeshAssetHandle mesh;
		TextureAssetHandle texture;
		PixelShaderAssetHandle pixelShader;
		VertexShaderAssetHandle vertexShader;
		bool useSkyBoxFlooring = false;
		bool useUnityShader = false;
	};

	void Initialize(SkyBoxComponent& skyBoxComponent, const Blackboard& blackboard);

	void RegisterSkyBoxComponent();

}