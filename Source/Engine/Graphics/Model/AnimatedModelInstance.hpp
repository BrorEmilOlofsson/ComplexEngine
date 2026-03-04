#pragma once
#include "Engine/Math/Transform3.hpp"
#include "Engine/Asset/TextureAsset.hpp"
#include "Engine/Asset/MeshAsset.hpp"
#include "Engine/Asset/AnimatedModelAsset.hpp"
#include "Engine/Graphics/GraphicsConstants.hpp"
#include <limits>

namespace CLX
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