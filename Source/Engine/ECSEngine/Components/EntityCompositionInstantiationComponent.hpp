#pragma once
#include <vector>
#include <any>
#include "Engine/Asset/AssetTypes/EntityCompositionAsset.hpp"
#include "Engine/ECS/EntityID.hpp"
#include "Engine/Reflection/PropertyPath.hpp"

namespace CLX
{

	struct EntityCompositionOverride final
	{
		PropertyPath path;
		std::any value;
	};

	struct EntityCompositionInstantiationComponent final
	{
		EntityCompositionAssetHandle asset;
        EntityID mappedEntityID = InvalidEntityID;
        std::vector<EntityCompositionOverride> overrides;
	};

	void RegisterEntityCompositionInstantiationComponent();
}