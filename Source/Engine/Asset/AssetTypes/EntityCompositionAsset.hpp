#pragma once
#include "Engine/Asset/Asset.hpp"
#include "Engine/Asset/AssetHandle.hpp"

#include "Engine/ECS/EntityComposition.hpp"

namespace CLX
{
	using EntityCompositionAsset = Asset<EntityComposition>;
	using EntityCompositionAssetHandle = AssetHandle<EntityComposition>;
}