#pragma once
#include "Utility/Math/Transform3.hpp"
#include "Utility/Asset/TextureAsset.hpp"
#include "Utility/Asset/MeshAsset.hpp"
#include "Utility/Asset/PixelShaderAsset.hpp"
#include "Utility/Asset/VertexShaderAsset.hpp"

namespace Simple
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