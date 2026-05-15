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
		EntityCompositionAssetHandle parentComposition;
	};

	class EntityCompositionInstantiationManager final
	{
	public:

		/*std::vector<EntityCompositionInstantiation>& GetInstantiations(EntityCompositionAssetHandle entityCompositionAssetHandle)
		{
            return map[entityCompositionAssetHandle];
		}*/

		std::span<const EntityCompositionInstantiation> GetInstantiations(EntityCompositionAssetHandle entityCompositionAssetHandle) const
		{
			return map.at(entityCompositionAssetHandle);
		}

		void Add(EntityCompositionAssetHandle entityCompositionAssetHandle, EntityCompositionInstantiation instantiation)
		{
            map[entityCompositionAssetHandle].push_back(std::move(instantiation));
		}

        void Clear()
        {
            map.clear();
        }

	private:

		std::map<EntityCompositionAssetHandle, std::vector<EntityCompositionInstantiation>> map;
	};
}