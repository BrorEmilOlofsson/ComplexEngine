#pragma once
#include "Engine/Math/Transform3.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/TransformHierarchyComponent.hpp"
#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include <variant>

namespace Simple
{

	template<typename T>
	[[nodiscard]] constexpr bool Contains(const std::vector<T>& a, const T& value)
	{
		return std::ranges::find(a, value) != end(a);
	}

	inline Transform GetWorldTransform(const ECS& ecs, const EntityID entityID)
	{
		const TransformComponent& transformComponent = *ecs.GetComponent<TransformComponent>(entityID);

		if (const TransformHierarchyComponent* hierarchyComponent = ecs.GetComponent<TransformHierarchyComponent>(entityID))
		{
			if (hierarchyComponent->parent != InvalidEntityID)
			{
				return transformComponent.transform.ToWorld(GetWorldTransform(ecs, hierarchyComponent->parent));
			}
		}

		return transformComponent.transform;
	}

	inline Transform GetLocalTransform(const ECS& ecs, const EntityID entityID, const Transform& worldTransform)
	{
		if (const TransformHierarchyComponent* hierarchyComponent = ecs.GetComponent<TransformHierarchyComponent>(entityID))
		{
			if (hierarchyComponent->parent != InvalidEntityID)
			{
				return Transform::FromMatrix(ToLocalSpace(worldTransform.GetMatrix(), GetWorldTransform(ecs, hierarchyComponent->parent).GetMatrix()));
			}
		}

		return worldTransform;
	}

	inline std::vector<EntityID> GetParents(const ECS& ecs, const EntityID entityID)
	{
		std::vector<EntityID> parents;

		if (const auto hierarchyComponent = ecs.GetComponent<TransformHierarchyComponent>(entityID))
		{
			if (hierarchyComponent->parent != InvalidEntityID)
			{
				parents.push_back(hierarchyComponent->parent);
				auto p = GetParents(ecs, hierarchyComponent->parent);
				parents.insert(end(parents), begin(p), end(p));
			}
		}
		return parents;
	}

	inline bool IsEntityDescendant(const ECS& ecs, const EntityID isChildID, const EntityID parentID)
	{
		const TransformHierarchyComponent* hiererchyChild = ecs.GetComponent<TransformHierarchyComponent>(isChildID);
		if (hiererchyChild == nullptr)
		{
			return false;
		}

		if (hiererchyChild->parent == parentID)
		{
			return true;
		}

		if (hiererchyChild->parent == InvalidEntityID)
		{
			return false;
		}

		return IsEntityDescendant(ecs, hiererchyChild->parent, parentID);
	}

	inline bool IsEntityChildable(const ECS& ecs, const EntityID parentID, const EntityID childID)
	{
		if (parentID == InvalidEntityID || childID == InvalidEntityID)
		{
			return false;
		}
		if (parentID == childID)
		{
			return false;
		}
		if (IsEntityDescendant(ecs, parentID, childID))
		{
			return false;
		}
		return true;
	}

	struct LastIndex {};
	struct FirstIndex {};
	struct Index { std::size_t mIndex = 0; };
	using ChildIndexSetting = std::variant<Index, FirstIndex, LastIndex>;

	inline bool SetParentEntity(ECS& ecs, const EntityID childID, const EntityID parentID, ChildIndexSetting indexSetting = LastIndex{})
	{
		if (parentID == InvalidEntityID)
		{
			auto hierarchyComponent = ecs.GetComponent<TransformHierarchyComponent>(childID);
			if (hierarchyComponent->parent != InvalidEntityID)
			{
				auto parentHierarchyComponent = ecs.GetComponent<TransformHierarchyComponent>(hierarchyComponent->parent);
				std::erase(parentHierarchyComponent->children, childID);
			}
			hierarchyComponent->parent = InvalidEntityID;
		}
		if (!IsEntityChildable(ecs, parentID, childID))
		{
			return false;
		}
		{
			ecs.InsertComponent<TransformHierarchyComponent>(parentID);
		}
		TransformHierarchyComponent& hierachyComponentChild = ecs.InsertComponent<TransformHierarchyComponent>(childID);
		TransformHierarchyComponent& hierarchyComponentParent = *ecs.GetComponent<TransformHierarchyComponent>(parentID);

		if (Contains(hierarchyComponentParent.children, childID))
		{
			return false;
		}

		const EntityID previousParentID = hierachyComponentChild.parent;
		if (previousParentID != InvalidEntityID)
		{
			TransformHierarchyComponent& hierarchyComponentPreviousParent = *ecs.GetComponent<TransformHierarchyComponent>(previousParentID);

			std::erase(hierarchyComponentPreviousParent.children, childID);
		}
		hierachyComponentChild.parent = parentID;
		if (std::holds_alternative<FirstIndex>(indexSetting))
		{
			hierarchyComponentParent.children.insert(begin(hierarchyComponentParent.children), childID);
		}
		else if (std::holds_alternative<Index>(indexSetting))
		{
			const std::size_t index = std::get<Index>(indexSetting).mIndex;
			if (index >= hierarchyComponentParent.children.size())
			{
				hierarchyComponentParent.children.push_back(childID);
			}
			else
			{
				hierarchyComponentParent.children.insert(begin(hierarchyComponentParent.children) + index, childID);
			}
		}
		else
		{
			hierarchyComponentParent.children.push_back(childID);
		}

		return true;
	}

	/*inline void SetParentEntity(ECS& ecs, const EntityID entityID, const EntityID newParentID)
	{
		if (entityID == InvalidEntityID)
		{
			throw std::invalid_argument("EntityID is invalid");
			return;
		}
		if (newParentID == InvalidEntityID)
		{
			auto hierarchyComponent = ecs.GetComponent<TransformHierarchyComponent>(entityID);
			auto parentHierarchyComponent = ecs.GetComponent<TransformHierarchyComponent>(hierarchyComponent->parent);
			std::erase(parentHierarchyComponent->children, entityID);
			hierarchyComponent->parent = InvalidEntityID;
		}
		else
		{
			AddChildEntity(ecs, newParentID, entityID);
		}
	}*/
}