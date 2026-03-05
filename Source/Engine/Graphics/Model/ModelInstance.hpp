#pragma once
#include "Engine/Math/Transform3.hpp"
#include "Engine/Asset/AssetTypes/TextureAsset.hpp"
#include "Engine/Asset/AssetTypes/MeshAsset.hpp"
#include "Engine/Asset/AssetTypes/ModelAsset.hpp"
#include <limits>

namespace CLX
{

	struct ModelInstance final
	{
		Transform transform;
		MeshAssetHandle mesh;
		ModelAssetHandle model;
		TextureAssetHandle albedoTexture;
		TextureAssetHandle normalTexture;
		TextureAssetHandle materialTexture;
		uint32_t objectID = std::numeric_limits<uint32_t>::max();
	};
}