#pragma once
#include "Engine/Math/Transform3.hpp"
#include "Engine/Asset/TextureAsset.hpp"
#include "Engine/Asset/MeshAsset.hpp"
#include "Engine/Asset/ModelAsset.hpp"
#include <limits>

namespace Simple
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