#pragma once
#include <vector>
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/TransformHierarchyComponent.hpp"

namespace CLX
{
	[[nodiscard]] std::vector<EntityID> GetRootEntities(const ECS& ecs);

	[[nodiscard]] inline std::span<const EntityID> GetEntityChildren(const ECS& ecs, const EntityID entityID)
	{
		return ecs.GetComponent<TransformHierarchyComponent>(entityID)->children;
	}

	[[nodiscard]] inline EntityID GetParentEntity(const ECS& ecs, const EntityID entityID)
	{
		return ecs.GetComponent<TransformHierarchyComponent>(entityID)->parent;
	}

	template<typename T>
	[[nodiscard]] bool HasAnyAncestorComponent(const ECS& ecs, const EntityID entityID)
	{
		if (ecs.HasComponent<T>(entityID))
		{
			return true;
		}

		EntityID parentID = GetParentEntity(ecs, entityID);
		while (parentID != InvalidEntityID)
		{
			if (ecs.HasComponent<T>(entityID))
			{
				return true;
			}
			parentID = GetParentEntity(ecs, entityID);
		}

		return false;
	}

	[[nodiscard]] inline std::vector<EntityID> GetEntityAncestors(const ECS& ecs, const EntityID entityID, bool includeSelf = false)
	{
		std::vector<EntityID> ancestors;
		if (includeSelf)
		{
			ancestors.push_back(entityID);
		}

		EntityID parent = GetParentEntity(ecs, entityID);
		while (parent != InvalidEntityID)
		{
			ancestors.push_back(parent);
			parent = GetParentEntity(ecs, parent);
		}

		return ancestors;
	}
}