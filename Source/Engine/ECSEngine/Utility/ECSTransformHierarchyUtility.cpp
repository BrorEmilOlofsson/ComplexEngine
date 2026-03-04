#include "Engine/Precompiled/EnginePch.hpp"
#include "ECSTransformHierarchyUtility.hpp"

namespace CLX
{

	std::vector<EntityID> GetRootEntities(const ECS& ecs)
	{
		std::vector<EntityID> rootEntities;
		for (auto entityView : ecs.ViewEntities())
		{
			rootEntities.push_back(entityView.GetEntityID());
		}
		ecs.ForEach([&rootEntities](const EntityID entityID, const TransformHierarchyComponent& hierarchyComponent)
			{
				if (hierarchyComponent.parent != InvalidEntityID)
				{
					std::erase(rootEntities, entityID);
				}
			});
		return rootEntities;
	}
}