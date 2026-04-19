#pragma once
#include <vector>

#include "Engine/Utility/Index.hpp"

#include "Engine/Math/Transform3.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/TransformHierarchyComponent.hpp"
#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Engine/ECSEngine/Components/NameComponent.hpp"

namespace CLX
{

    template<typename T>
    [[nodiscard]] constexpr bool Contains(const std::vector<T>& a, const T& value)
    {
        return std::ranges::find(a, value) != end(a);
    }


    inline bool IsEntityDescendant(const ECS& ecs, const EntityID isChildID, const EntityID parentID)
    {
        const TransformHierarchyComponent* hiererchyChild = ecs.GetComponent<TransformHierarchyComponent>(isChildID);
        ASSERT(hiererchyChild != nullptr);

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
        else if (std::holds_alternative<LastIndex>(indexSetting))
        {
            hierarchyComponentParent.children.push_back(childID);
        }
        else
        {
            ASSERT_NEW(false, "Invalid ChildIndexSetting");
        }

        return true;
    }

    [[nodiscard]] inline std::vector<EntityID> GetRootEntities(const ECS& ecs)
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

    [[nodiscard]] inline std::span<const EntityID> GetEntityChildren(const ECS& ecs, const EntityID entityID)
    {
        return ecs.GetComponent<TransformHierarchyComponent>(entityID)->children;
    }

    [[nodiscard]] inline std::vector<EntityID> GetAllEntityChildren(const ECS& ecs, const EntityID entityID)
    {
        std::vector<EntityID> allChildren;
        auto* tch = ecs.GetComponent<TransformHierarchyComponent>(entityID);
        ASSERT(tch != nullptr);
        const auto& children = tch->children;
        allChildren.insert(allChildren.end(), children.begin(), children.end());
        for (const auto& child : children)
        {
            const auto& grandChildren = GetAllEntityChildren(ecs, child);
            allChildren.insert(allChildren.end(), grandChildren.begin(), grandChildren.end());
        }
        return allChildren;
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

    template<typename T>
    [[nodiscard]] inline bool HasAncestorComponent(const ECS& ecs, const EntityID entityID, const bool includeSelf)
    {
        EntityID entityToCheck = includeSelf ? entityID : GetParentEntity(ecs, entityID);
        while (entityToCheck != InvalidEntityID)
        {
            if (ecs.HasComponent<T>(entityToCheck))
            {
                return true;
            }
            entityToCheck = GetParentEntity(ecs, entityToCheck);
        }
        return false;
    }

    inline const Transform& GetEntityTransform(const ECS& ecs, const EntityID entityID)
    {
        const TransformComponent* transformComponent = ecs.GetComponent<TransformComponent>(entityID);
        ASSERT(transformComponent != nullptr);
        return transformComponent->transform;
    }

    inline Transform GetEntityWorldTransform(const ECS& ecs, const EntityID entityID)
    {
        const Transform& localTransform = GetEntityTransform(ecs, entityID);
        const EntityID parentID = GetParentEntity(ecs, entityID);
        if (parentID != InvalidEntityID)
        {
            return localTransform.ToWorld(GetEntityWorldTransform(ecs, parentID));
        }

        return localTransform;
    }

    inline Transform GetEntityLocalTransform(const ECS& ecs, const EntityID entityID, const Transform& worldTransform)
    {
        const TransformHierarchyComponent* hierarchyComponent = ecs.GetComponent<TransformHierarchyComponent>(entityID);
        ASSERT(hierarchyComponent != nullptr);
        if (hierarchyComponent->parent != InvalidEntityID)
        {
            return Transform::FromMatrix(ToLocalSpace(worldTransform.GetMatrix(), GetEntityWorldTransform(ecs, hierarchyComponent->parent).GetMatrix()));
        }

        return worldTransform;
    }

    inline std::vector<EntityID> GetEntityParents(const ECS& ecs, const EntityID entityID)
    {
        std::vector<EntityID> parents;

        const auto hierarchyComponent = ecs.GetComponent<TransformHierarchyComponent>(entityID);
        ASSERT(hierarchyComponent != nullptr);
        if (hierarchyComponent->parent != InvalidEntityID)
        {
            parents.push_back(hierarchyComponent->parent);
            auto p = GetEntityParents(ecs, hierarchyComponent->parent);
            parents.insert(end(parents), begin(p), end(p));
        }

        return parents;
    }

    [[nodiscard]] inline const std::string& GetEntityName(const ECS& ecs, const EntityID entityID)
    {
        const NameComponent* nameComponent = ecs.GetComponent<NameComponent>(entityID);
        ASSERT(nameComponent != nullptr);
        return nameComponent->name;
    }

    [[nodiscard]] inline void SetEntityName(ECS& ecs, const EntityID entityID, std::string name)
    {
        NameComponent* nameComponent = ecs.GetComponent<NameComponent>(entityID);
        ASSERT(nameComponent != nullptr);
        nameComponent->name = std::move(name);
    }
}