#include "Engine/Precompiled/EnginePch.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECS/EntityComposition.hpp"
#include "Engine/ECSEngine/Components/TransformHierarchyComponent.hpp"

namespace Simple
{

	static void UpdateEntityIDs(const ECS& previousECS, ECS& ecs, const EntityIDConverter& entityIDConverter)
	{
		for (auto e : previousECS.ViewEntities())
		{
			const EntityID newEntityID = entityIDConverter[e.GetEntityID()];
			auto entityView = ecs.ViewEntity(newEntityID);
			TransformHierarchyComponent* hierarchyComponent = entityView.GetComponent<TransformHierarchyComponent>();
			if (hierarchyComponent)
			{
				if (hierarchyComponent->parent != InvalidEntityID)
				{
					hierarchyComponent->parent = entityIDConverter[hierarchyComponent->parent];
				}

				for (std::size_t i = 0; i < hierarchyComponent->children.size(); i++)
				{
					hierarchyComponent->children[i] = entityIDConverter[hierarchyComponent->children[i]];
				}
			}
		}
	}

	void MergeEntityComposition(const EntityComposition& entityComposition, ECS& ecs)
	{
		const EntityIDConverter entityConverter = ecs.MergeEntities(entityComposition.GetECS());
		UpdateEntityIDs(entityComposition.GetECS(), ecs, entityConverter);
	}
}