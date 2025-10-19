#pragma once
#include "Utility/Math/Transform3.hpp"
#include "Utility/Asset/TextureAsset.hpp"
#include "Utility/Asset/MeshAsset.hpp"
#include <limits>

namespace Simple
{

	struct ModelInstance final
	{
		Transform transform;
		MeshAssetHandle mesh;
		TextureAssetHandle albedoTexture;
		TextureAssetHandle normalTexture;
		TextureAssetHandle materialTexture;
		uint32_t objectID = std::numeric_limits<uint32_t>::max();
	};
}