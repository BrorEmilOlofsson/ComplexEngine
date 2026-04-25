#include "Engine/Precompiled/EnginePch.hpp"
#include "ECSEntityCompositionUtility.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECS/EntityComposition.hpp"
#include "Engine/ECSEngine/Components/TransformHierarchyComponent.hpp"
#include "Engine/ECSEngine/Utility/ECSUtilityFunctions.hpp"

namespace CLX
{

	void UpdateEntityIDs(const ECS& previousECS, ECS& ecs, const EntityIDConverter& entityIDConverter, const DataTypeRegistry& dataTypeRegistry)
	{
		for (auto entityView : previousECS.ViewEntities())
		{
			const EntityID newEntityID = entityIDConverter.ConvertToTarget(entityView.GetEntityID());
			auto newEntityView = ecs.ViewEntity(newEntityID);
			TransformHierarchyComponent& hierarchyComponent = *newEntityView.GetComponent<TransformHierarchyComponent>();

			if (hierarchyComponent.parent != InvalidEntityID)
			{
				hierarchyComponent.parent = entityIDConverter.ConvertToTarget(hierarchyComponent.parent);
			}

			for (std::size_t i = 0; i < hierarchyComponent.children.size(); i++)
			{
				hierarchyComponent.children[i] = entityIDConverter.ConvertToTarget(hierarchyComponent.children[i]);
			}

            RemapComponentEntityIDs(ecs, newEntityID, entityIDConverter.GetSourceToTargetMap(), dataTypeRegistry);
		}
	}
}