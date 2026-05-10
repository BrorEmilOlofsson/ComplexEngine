#pragma once
#include <map>
#include <vector>

#include "Engine/Asset/AssetTypes/EntityCompositionAsset.hpp"
#include "Engine/ECS/EntityID.hpp"
#include "Engine/ECS/ECSHandle.hpp"

namespace CLX
{
	struct EntityCompositionInstantiation final
	{
		EntityID rootEntityID;
		ECSHandle ecsHandle;
	};

	class EntityCompositionInstantiationManager final
	{
	public:
		std::vector<EntityCompositionInstantiation>& Get(EntityCompositionAssetHandle entityCompositionAssetHandle)
		{
            return map[entityCompositionAssetHandle];
		}

		std::map<EntityCompositionAssetHandle, std::vector<EntityCompositionInstantiation>> map;
	};
}