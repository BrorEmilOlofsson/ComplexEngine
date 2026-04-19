#pragma once
#include "Engine/Asset/AssetTypes/EntityCompositionAsset.hpp"

namespace CLX
{
	struct EntityCompositionInstantiationComponent final
	{
		EntityCompositionAssetHandle asset;
        EntityID mappedEntityID = InvalidEntityID;
	};

	void RegisterEntityCompositionInstantiationComponent();
}