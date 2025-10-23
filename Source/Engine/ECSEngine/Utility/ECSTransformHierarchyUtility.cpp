#include "Engine/Precompiled/EnginePch.hpp"
#include "ECSTransformHierarchyUtility.hpp"

namespace Simple
{

	std::vector<EntityID> GetRootEntities(const ECS& ecs)
	{
		std::vector<EntityID> rootEntities;
		for (auto entityView : ecs.ViewEntities())
		{
			rootEntities.push_back(entityView.GetEntityID());
		}
		for (auto [entityID, hierarchyComponent] : ecs.ViewUsingEntityID<TransformHierarchyComponent>())
		{
			if (hierarchyComponent.parent != InvalidEntityID)
			{
				std::erase(rootEntities, entityID);
			}
		}
		return rootEntities;
	}
}