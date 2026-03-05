#pragma once
#include <limits>
#include <array>
#include "Engine/Math/Transform3.hpp"
#include "Engine/Asset/AssetTypes/TextureAsset.hpp"
#include "Engine/Asset/AssetTypes/MeshAsset.hpp"
#include "Engine/Asset/AssetTypes/AnimatedModelAsset.hpp"
#include "Engine/Graphics/GraphicsConstants.hpp"
#include "Engine/Math/Matrix4x4.hpp"

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