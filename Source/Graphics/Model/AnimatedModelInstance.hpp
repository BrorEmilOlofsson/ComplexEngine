#pragma once
#include "Utility/Math/Transform3.hpp"
#include "Utility/Asset/TextureAsset.hpp"
#include "Utility/Asset/MeshAsset.hpp"
#include "Utility/Asset/AnimatedModelAsset.hpp"
#include "Graphics/GraphicsConstants.hpp"
#include <limits>

namespace Simple
{

	struct AnimatedModelInstance final
	{
		Transform transform;
		AnimatedModelAssetHandle animatedModel;
		TextureAssetHandle albedoTexture;
		TextureAssetHandle normalTexture;
		TextureAssetHandle materialTexture;
		std::array<Matrix4x4f, GlobalMaxBones> boneMatrices;
		uint32_t objectID = std::numeric_limits<uint32_t>::max();
	};
}