#include "Editor/Precompiled/EditorPch.hpp"
#include "ECSEditorFunctions.hpp"
#include "Editor/Command/Core/EditorCommandTracker.hpp"
#include "Engine/ECSEngine/Components/NameComponent.hpp"
#include "Engine/ECSEngine/Components/TransformHierarchyComponent.hpp"
#include "Editor/Command/Commands/SetDataPtrCommand.hpp"
#include "Engine/ECSEngine/Components/MeshComponent.hpp"
#include "Engine/ECSEngine/Components/EditorComponent.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformUtility.hpp"
#include "Engine/Utility/Camera.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Engine/Reflection/DataTypeRegistry.hpp"
#include "Engine/Asset/AssetTypes/EntityCompositionAsset.hpp"
#include "Engine/ECS/EntityComposition.hpp"
#include "Engine/ECSEngine/Components/EntityCompositionComponent.hpp"
#include "Engine/ECSEngine/Utility/ECSEntityCompositionUtility.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformHierarchyUtility.hpp"
#include "Engine/Utility/Visitor.hpp"
#include "Engine/Reflection/PropertyPath.hpp"

namespace CLX
{

    constexpr bool CaseInsensitiveContains(const std::string& str, const std::string& searchString)
    {
        auto it = std::search(
            str.begin(), str.end(),
            searchString.begin(), searchString.end(),
            [](char ch1, char ch2) { return std::tolower(ch1) == std::tolower(ch2); }
        );
        return (it != str.end());
    }

    void InsertRange(std::ranges::range auto& range, std::ranges::range auto&& range2)
    {
        range.insert(range.end(), range2.begin(), range2.end());
    }

    static std::size_t GetEntityIndexInParent(const ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities)
    {
        const EntityID parentID = GetParentEntity(ecs, entityID);
        if (parentID == InvalidEntityID)
        {
            auto it = std::ranges::find(rootEntities, entityID);
            ASSERT(it != end(rootEntities));
            return std::ranges::distance(begin(rootEntities), it);
        }
        else
        {
            auto parentTransformComponent = ecs.GetComponent<TransformHierarchyComponent>(parentID);
            ASSERT(parentTransformComponent != nullptr);
            auto it = std::ranges::find(parentTransformComponent->children, entityID);
            ASSERT(it != end(parentTransformComponent->children));
            return std::ranges::distance(begin(parentTransformComponent->children), it);
        }
    }

    static void SwapEntities(ECS& ecs1, ECS& ecs2, EntityID entityID1, EntityID entityID2)
    {
        const EntityID tempCopy = ecs1.CopyEntity(entityID1, ecs2);
        ecs1.ReplaceEntity(entityID1, ecs2, entityID2);
        ecs2.ReplaceEntity(entityID2, ecs2, tempCopy);
        ecs2.DestroyEntity(tempCopy);
    }

    static void DestroyEntityAndChildren(ECS& ecs, const EntityID entityID)
    {
        auto children = GetAllEntityChildren(ecs, entityID);
        for (auto child : children)
        {
            ecs.DestroyEntity(child);
        }
        ecs.DestroyEntity(entityID);
    }

    [[nodiscard]] bool HasVariableEntity(const std::type_info& typeInfo)
    {
        return typeInfo == typeid(EntityID) || typeInfo == typeid(std::vector<EntityID>);
    }

    static void RemapEntityIDs(ECS& ecs, const EntityID newEntityID, const std::map<EntityID, EntityID>& oldToNewEntityIDMap, const DataTypeRegistry& dataTypeRegistry)
    {
        for (auto [typeinfo, componentPtr] : ECS::EntityView(&ecs, newEntityID))
        {
            const DataType* dataType = dataTypeRegistry.Find(GetDataTypeID(typeinfo));
            ASSERT(dataType != nullptr);
            auto& members = dataType->memberVariables;

            auto membersFiltered = members | std::views::filter([&dataTypeRegistry](const DataTypeMemberVariable& member)
                {
                    const auto& dataType = dataTypeRegistry.Find(member.dataTypeID);
                    ASSERT(dataType != nullptr);
                    return HasVariableEntity(dataType->typeInfo);
                });

            auto memberPair = membersFiltered | std::views::transform([&dataTypeRegistry, &componentPtr](const DataTypeMemberVariable& member)
                {
                    return std::pair<const std::type_info&, void*>{ dataTypeRegistry.Find(member.dataTypeID)->typeInfo.get(), componentPtr + std::get<ByteOffset>(member.memberType) };
                })
                | std::ranges::to<std::vector>();

            for (const auto& [memberTypeInfo, memberPtr] : memberPair)
            {
                if (memberTypeInfo == typeid(EntityID))
                {
                    EntityID& entityRef = *static_cast<EntityID*>(memberPtr);
                    if (oldToNewEntityIDMap.contains(entityRef))
                    {
                        entityRef = oldToNewEntityIDMap.at(entityRef);
                    }
                }
                else if (memberTypeInfo == typeid(std::vector<EntityID>))
                {
                    std::vector<EntityID>& entityVectorRef = *static_cast<std::vector<EntityID>*>(memberPtr);
                    for (EntityID& entityIDInVector : entityVectorRef)
                    {
                        if (oldToNewEntityIDMap.contains(entityIDInVector))
                        {
                            entityIDInVector = oldToNewEntityIDMap.at(entityIDInVector);
                        }
                    }
                }
                else
                {
                    ASSERT_NEW(false, "Unexpected type info for member variable when duplicating entity and children");
                }
            }
        }
    }

    static std::map<EntityID, EntityID> DuplicateEntityAndChildren(ECS& ecs, const EntityID entityID, const DataTypeRegistry& dataTypeRegistry)
    {
        const EntityID createdEntityID = ecs.DuplicateEntity(entityID);
        std::map<EntityID, EntityID> oldToNewEntityIDMap;
        oldToNewEntityIDMap[entityID] = createdEntityID;
        auto children = GetAllEntityChildren(ecs, entityID);

        for (auto child : children)
        {
            const EntityID createdChildEntityID = ecs.DuplicateEntity(child);
            oldToNewEntityIDMap[child] = createdChildEntityID;
        }

        // Remap parent-child relationships in the duplicated entities
        RemapEntityIDs(ecs, createdEntityID, oldToNewEntityIDMap, dataTypeRegistry);

        for (auto child : children)
        {
            const EntityID createdChildEntityID = oldToNewEntityIDMap.at(child);
            RemapEntityIDs(ecs, createdChildEntityID, oldToNewEntityIDMap, dataTypeRegistry);
        }

        return oldToNewEntityIDMap;
    }

    void AddEntityToRootEntities(const EntityID entityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
    {
        if (std::ranges::find(rootEntities, entityID) != end(rootEntities))
        {
            return;
        }
        struct AddEntityToRootEntitiesData final
        {
            EntityID entityID;
            std::reference_wrapper<std::vector<EntityID>> rootEntities;
        };

        AddEntityToRootEntitiesData data
        {
            .entityID = entityID,
            .rootEntities = rootEntities
        };

        auto doCommand = [](const AddEntityToRootEntitiesData& data)
            {
                data.rootEntities.get().push_back(data.entityID);
            };

        auto undoCommand = [](const AddEntityToRootEntitiesData& data)
            {
                data.rootEntities.get().pop_back();
            };

        commandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Add Entity To Root Entities"));
    }


    EntityID DuplicateEntityAndChildren(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, const DataTypeRegistry& dataTypeRegistry, EditorCommandTracker& commandTracker)
    {

        std::map<EntityID, EntityID> oldToNewEntityIDMap = DuplicateEntityAndChildren(ecs, entityID, dataTypeRegistry);
        const EntityID newEntityID = oldToNewEntityIDMap.at(entityID);


        struct DuplicateEntityAndChildrenData final
        {
            EntityID entityID;
            EntityID duplicatedEntityID;
            std::reference_wrapper<ECS> ecs;
            std::reference_wrapper<std::vector<EntityID>> rootEntities;
            EntityID parentID;
            std::reference_wrapper<const DataTypeRegistry> dataTypeRegistry;
        };


        DuplicateEntityAndChildrenData data
        {
            .entityID = entityID,
            .duplicatedEntityID = newEntityID,
            .ecs = ecs,
            .rootEntities = rootEntities,
            .parentID = GetParentEntity(ecs, entityID),
            .dataTypeRegistry = dataTypeRegistry
        };

        auto doCommand = [](const DuplicateEntityAndChildrenData& data)
            {
                DuplicateEntityAndChildren(data.ecs.get(), data.entityID, data.dataTypeRegistry);

                if (data.parentID == InvalidEntityID)
                {
                    data.rootEntities.get().push_back(data.duplicatedEntityID);
                }
                else
                {
                    TransformHierarchyComponent* parentTransformComponent = data.ecs.get().GetComponent<TransformHierarchyComponent>(data.parentID);
                    ASSERT(parentTransformComponent != nullptr);
                    parentTransformComponent->children.push_back(data.duplicatedEntityID);
                }

            };

        auto undoCommand = [](const DuplicateEntityAndChildrenData& data)
            {
                DestroyEntityAndChildren(data.ecs.get(), data.duplicatedEntityID);

                if (data.parentID == InvalidEntityID)
                {
                    data.rootEntities.get().pop_back();
                }
                else
                {
                    TransformHierarchyComponent* parentTransformComponent = data.ecs.get().GetComponent<TransformHierarchyComponent>(data.parentID);
                    ASSERT(parentTransformComponent != nullptr);
                    parentTransformComponent->children.pop_back();
                }
            };

        commandTracker.RegisterCommand(EditorCommand(data, doCommand, undoCommand, "Duplicate Entity and Children"));

        return newEntityID;
    }

    static EntityID CreateRootEntity(ECS& ecs, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
    {
        ecs;
        rootEntities;
        commandTracker;
        const EntityID createdEntityID = ecs.CreateEntity();

        struct CreateEntityData final
        {
            EntityID createdEntityID;
            std::reference_wrapper<ECS> ecs;
            std::reference_wrapper<std::vector<EntityID>> rootEntities;
            std::size_t rootIndex = 0;
        };

        CreateEntityData data
        {
             .createdEntityID = createdEntityID,
             .ecs = ecs,
             .rootEntities = rootEntities,
             .rootIndex = rootEntities.size()
        };

        auto doCommand = [](const CreateEntityData& data)
            {
                data.rootEntities.get().insert(begin(data.rootEntities.get()) + data.rootIndex, data.createdEntityID);
                data.ecs.get().ActivateEntity(data.createdEntityID);
            };

        auto undoCommand = [](const CreateEntityData& data)
            {
                data.rootEntities.get().erase(begin(data.rootEntities.get()) + data.rootIndex);
                data.ecs.get().DeactivateEntity(data.createdEntityID);
            };

        commandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Create Root Entity"));

        return createdEntityID;
    }

    EntityID CreateChildEntity(ECS& ecs, const EntityID parentID, EditorCommandTracker& commandTracker)
    {
        const EntityID createdEntityID = ecs.CreateEntity();
        struct CreateEntityData final
        {
            EntityID createdEntityID;
            std::reference_wrapper<ECS> ecs;
            EntityID parentID;
            std::size_t index = 0;
        };

        CreateEntityData data
        {
            .createdEntityID = createdEntityID,
            .ecs = ecs,
            .parentID = parentID,
            .index = ecs.GetComponent<TransformHierarchyComponent>(parentID)->children.size()
        };

        auto doCommand = [](const CreateEntityData& data)
            {
                TransformHierarchyComponent* parentComponent = data.ecs.get().GetComponent<TransformHierarchyComponent>(data.parentID);
                parentComponent->children.insert(begin(parentComponent->children) + data.index, data.createdEntityID);
                TransformHierarchyComponent* childComponent = data.ecs.get().GetComponent<TransformHierarchyComponent>(data.createdEntityID);
                childComponent->parent = data.parentID;
                data.ecs.get().ActivateEntity(data.createdEntityID);
            };

        auto undoCommand = [](const CreateEntityData& data)
            {
                TransformHierarchyComponent* parentComponent = data.ecs.get().GetComponent<TransformHierarchyComponent>(data.parentID);
                parentComponent->children.erase(begin(parentComponent->children) + data.index);
                TransformHierarchyComponent* childComponent = data.ecs.get().GetComponent<TransformHierarchyComponent>(data.createdEntityID);
                childComponent->parent = InvalidEntityID;
                data.ecs.get().DeactivateEntity(data.createdEntityID);
            };

        commandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Create Entity"));

        return createdEntityID;
    }

    EntityID CreateEntity(ECS& ecs, std::vector<EntityID>& rootEntities, const EntityID parentID, EditorCommandTracker& commandTracker)
    {
        if (parentID == InvalidEntityID)
        {
            return CreateRootEntity(ecs, rootEntities, commandTracker);
        }
        else
        {
            return CreateChildEntity(ecs, parentID, commandTracker);
        }
    }

    static void DestroyRootEntity(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
    {
        struct DestroyEntityData
        {
            std::reference_wrapper<ECS> ecs;
            EntityID entityID;
            std::size_t index = 0;
            std::reference_wrapper<std::vector<EntityID>> rootEntities;
        };

        DestroyEntityData data
        {
            .ecs = ecs,
            .entityID = entityID,
            
            .index = static_cast<std::size_t>(std::ranges::distance(begin(rootEntities), std::ranges::find(rootEntities, entityID))),
            .rootEntities = rootEntities
        };
        auto doCommand = [](const DestroyEntityData& data)
            {
                data.rootEntities.get().erase(begin(data.rootEntities.get()) + data.index);
                EntityID entityID = data.entityID;
                entityID.generation = std::numeric_limits<uint32_t>::max();
                data.ecs.get().DeactivateEntity(entityID);
            };

        auto undoCommand = [](const DestroyEntityData& data)
            {
                data.rootEntities.get().insert(begin(data.rootEntities.get()) + data.index, data.entityID);
                data.ecs.get().ActivateEntity(data.entityID);
            };

        commandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Destroy Root Entity"));
    }

    static void DestroyChildEntity(ECS& ecs, const EntityID entityID, EditorCommandTracker& commandTracker)
    {
        TransformHierarchyComponent* hierarchyComponent = ecs.GetComponent<TransformHierarchyComponent>(entityID);
        ASSERT(hierarchyComponent != nullptr);
        ASSERT(hierarchyComponent->parent != InvalidEntityID);

        struct DestroyEntityData final
        {
            EntityID parentID;
            EntityID childID;
            std::size_t index = 0;
            std::reference_wrapper<ECS> ecs;
        };

        TransformHierarchyComponent* parentChildrenTransformComponent = ecs.GetComponent<TransformHierarchyComponent>(hierarchyComponent->parent);
        ASSERT(parentChildrenTransformComponent != nullptr);

        DestroyEntityData data
        {
            .parentID = hierarchyComponent->parent,
            .childID = entityID,
            .index = static_cast<std::size_t>(std::ranges::distance(begin(parentChildrenTransformComponent->children), std::ranges::find(parentChildrenTransformComponent->children, entityID))),
            .ecs = ecs
        };

        auto doCommand = [](const DestroyEntityData& data)
            {
                TransformHierarchyComponent* parentChildrenTransformComponent = data.ecs.get().GetComponent<TransformHierarchyComponent>(data.parentID);
                std::erase(parentChildrenTransformComponent->children, data.childID);
                data.ecs.get().DeactivateEntity(data.childID);
            };

        auto undoCommand = [](const DestroyEntityData& data)
            {
                TransformHierarchyComponent* parentChildrenTransformComponent = data.ecs.get().GetComponent<TransformHierarchyComponent>(data.parentID);
                parentChildrenTransformComponent->children.insert(begin(parentChildrenTransformComponent->children) + data.index, data.childID);
                data.ecs.get().ActivateEntity(data.childID);
            };

        commandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Destroy Child Entity"));
    }

    void DestroyEntity(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
    {
        if (std::ranges::find(rootEntities, entityID) != end(rootEntities))
        {
            DestroyRootEntity(ecs, entityID, rootEntities, commandTracker);
        }
        else
        {
            DestroyChildEntity(ecs, entityID, commandTracker);
        }
    }

    void DestroyEntities(ECS& ecs, const std::ranges::range auto& entityIDs, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
    {
        commandTracker.BeginComposite("Destroy Entities");
        for (const EntityID entityID : entityIDs)
        {
            DestroyEntity(ecs, entityID, rootEntities, commandTracker);
        }
        commandTracker.EndComposite();
    }

    void DestroyEntities(ECS& ecs, const std::set<EntityID>& entityIDs, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
    {
        commandTracker.BeginComposite("Destroy Entities");
        for (const EntityID entityID : entityIDs)
        {
            DestroyEntity(ecs, entityID, rootEntities, commandTracker);
        }
        commandTracker.EndComposite();
    }

    void DestroyEntityAndChildren(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
    {
        auto allChildren = GetAllEntityChildren(ecs, entityID);
        allChildren.push_back(entityID);

        DestroyEntities(ecs, allChildren, rootEntities, commandTracker);
    }

    void DestroyEntitiesAndChildren(ECS& ecs, const std::set<EntityID>& entityIDs, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
    {
        std::set<EntityID> allEntityIDsToDestroy;

        for (EntityID entityID : entityIDs)
        {
            auto children = GetAllEntityChildren(ecs, entityID);
            allEntityIDsToDestroy.insert(entityID);
            allEntityIDsToDestroy.insert(children.begin(), children.end());
        }

        DestroyEntities(ecs, allEntityIDsToDestroy, rootEntities, commandTracker);
    }

    void SetEntitySelection(const std::set<EntityID>& entityIDs, std::set<EntityID>& selectedEntityIDs, EditorCommandTracker& commandTracker)
    {
        SetDataPtrCommand<std::set<EntityID>> command;
        command.mNewValue = entityIDs;
        command.mOldValue = selectedEntityIDs;
        command.mPtr = &selectedEntityIDs;

        commandTracker.ExecuteCommand(EditorCommand(command, "Select Entities"));
    }

    void SetEntitySelection(const EntityID entityIDs, std::set<EntityID>& selectedEntityIDs, EditorCommandTracker& commandTracker)
    {
        SetEntitySelection(std::set<EntityID>{ entityIDs }, selectedEntityIDs, commandTracker);
    }

    void ClearEntitySelection(std::set<EntityID>& selectedEntityIDs, EditorCommandTracker& commandTracker)
    {
        SetEntitySelection(std::set<EntityID>{}, selectedEntityIDs, commandTracker);
    }

    void AppendEntitySelection(const EntityID entityID, std::set<EntityID>& selectedEntityIDs, EditorCommandTracker& commandTracker)
    {
        if (selectedEntityIDs.contains(entityID))
        {
            return;
        }

        std::set newSelectedEntities = selectedEntityIDs;
        newSelectedEntities.insert(entityID);
        SetEntitySelection(newSelectedEntities, selectedEntityIDs, commandTracker);
    }

    void AppendEntitySelection(const std::set<EntityID>& entityIDs, std::set<EntityID>& selectedEntityIDs, EditorCommandTracker& commandTracker)
    {
        std::size_t selectedEntityCount = selectedEntityIDs.size();
        std::set newSelectedEntities = selectedEntityIDs;
        newSelectedEntities.insert(entityIDs.begin(), entityIDs.end());
        if (newSelectedEntities.size() == selectedEntityCount)
        {
            return;
        }
        SetEntitySelection(newSelectedEntities, selectedEntityIDs, commandTracker);
    }

    /*void SelectEntity(const EntityID entityID, EntityID& selectedEntityID, EditorCommandTracker& commandTracker)
    {
        if (entityID == selectedEntityID)
        {
            return;
        }

        SetDataPtrCommand<EntityID> command;

        command.mNewValue = entityID;
        command.mOldValue = selectedEntityID;
        command.mPtr = &selectedEntityID;

        commandTracker.ExecuteCommand(EditorCommand(command, "Select Entity"));
    }*/

    static void AddComponentToEntity(ECS& ecs, const EntityID entityID, const DataTypeID dataTypeID, const std::string& componentName/*, ECS& ecsBuffer*/, EditorCommandTracker& commandTracker)
    {
        struct AddComponentData final
        {
            std::reference_wrapper<ECS> ecs;
            EntityID entityID;
            DataTypeID dataTypeID;
        };

        AddComponentData data
        {
            .ecs = ecs,
            .entityID = entityID,
            .dataTypeID = dataTypeID
        };

        auto doCommand = [](const AddComponentData& data)
            {
                data.ecs.get().GetRegistry().GetComponentType(data.dataTypeID).addComponentFunction(data.ecs.get(), data.entityID, nullptr);

            };

        auto undoCommand = [](const AddComponentData& data)
            {
                data.ecs.get().RemoveComponent(data.entityID, data.dataTypeID);
            };

        commandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Add Component (" + componentName + ")"));
    }

    [[nodiscard]] EditorAction CreateAddComponentToEntityAction(ECS& ecs, const EntityID entityID, const DataTypeID dataTypeID, const std::string& componentName)
    {
        return [&ecs, entityID, dataTypeID, componentName](EditorCommandTracker& commandTracker)
            {
                AddComponentToEntity(ecs, entityID, dataTypeID, componentName, commandTracker);
            };
    }

    void RemoveComponent(ECS& ecs, const EntityID entityID, const DataTypeID& dataTypeID, ECS& ecsBuffer, EditorCommandTracker& commandTracker)
    {
        const EntityID copyEntityID = ecs.CopyEntity(entityID, ecsBuffer);
        ecs.RemoveComponent(entityID, dataTypeID);


        struct RemoveComponentData final
        {
            std::reference_wrapper<ECS> ecs;
            EntityID entityID;
            std::reference_wrapper<ECS> ecsBuffer;
            EntityID copyEntityID;
        };

        RemoveComponentData data
        {
            .ecs = ecs,
            .entityID = entityID,
            .ecsBuffer = ecsBuffer,
            .copyEntityID = copyEntityID
        };

        auto command = [](const RemoveComponentData& data)
            {
                SwapEntities(data.ecs.get(), data.ecsBuffer.get(), data.entityID, data.copyEntityID);
            };

        commandTracker.RegisterCommand(EditorCommand(data, command, command, "Remove Component"));
    }

    void ShowEntityAddButtons(ECS& ecs, std::set<EntityID>& selectedEntityIDs, std::vector<EntityID>& rootEntities,
        EditorCommandTracker& commandTracker, const std::string& imGuiTag, const EntityID defaultParent)
    {
        const std::string addButton = "Add" + imGuiTag;
        const std::string addSceneObjectButton = "Add Scene Object" + imGuiTag;

        if (ImGui::Button(addButton.c_str()))
        {
            ImGui::OpenPopup(addSceneObjectButton.c_str());
        }

        if (ImGui::BeginPopup(addSceneObjectButton.c_str()))
        {
            const std::string addEntityItem = "Add Entity" + imGuiTag;

            if (ImGui::MenuItem(addEntityItem.c_str()))
            {
                commandTracker.BeginComposite("Create Entity + Select Entity");

                const EntityID createdEntityID = CreateEntity(ecs, rootEntities, defaultParent, commandTracker);

                SetEntitySelection(createdEntityID, selectedEntityIDs, commandTracker);

                commandTracker.EndComposite();
            }

            const std::string addCubeItem = "Add Cube" + imGuiTag;
            if (ImGui::MenuItem(addCubeItem.c_str()))
            {
                commandTracker.BeginComposite("Create Entity + Select Entity");

                const EntityID createdEntityID = CreateEntity(ecs, rootEntities, defaultParent, commandTracker);

                AddComponentToEntity(ecs, createdEntityID, GetDataTypeID(typeid(MeshComponent)), "Mesh Component", commandTracker);

                SetEntitySelection(createdEntityID, selectedEntityIDs, commandTracker);

                commandTracker.EndComposite();
            }

            ImGui::EndPopup();
        }
    }

    static void ReorderInternal(std::vector<EntityID>& entities, const std::size_t oldIndex, const std::size_t newIndex)
    {
        auto first = entities.begin();
        auto from = first + oldIndex;
        auto to = first + newIndex;

        if (oldIndex < newIndex)
        {
            std::rotate(from, from + 1, to + 1);
        }
        else
        {
            std::rotate(to, from, from + 1);
        }
    }

    void ReorderEntity(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, std::size_t newIndex, EditorCommandTracker& commandTracker)
    {
        ASSERT(entityID != InvalidEntityID);

        struct ReorderEntityData final
        {
            std::reference_wrapper<ECS> ecs;
            EntityID entityID;
            std::reference_wrapper<std::vector<EntityID>> rootEntities;
            std::size_t newIndex = std::numeric_limits<std::size_t>::max();
            std::size_t oldIndex = std::numeric_limits<std::size_t>::max();
        };

        const EntityID parentID = GetParentEntity(ecs, entityID);

        const std::size_t oldIndex = GetEntityIndexInParent(ecs, entityID, rootEntities);

        if (oldIndex < newIndex)
        {
            // Because the entity will be removed from the old index before being inserted at the new index, we need to adjust the new index accordingly
            newIndex -= 1;
        }

        if (oldIndex == newIndex)
        {
            return;
        }

        ReorderEntityData data
        {
            .ecs = ecs,
            .entityID = entityID,
            .rootEntities = rootEntities,
            .newIndex = newIndex,
            .oldIndex = oldIndex
        };

        auto doCommand = [](const ReorderEntityData& data)
            {
                const EntityID parentID = GetParentEntity(data.ecs, data.entityID);
                auto& rootEntities = data.rootEntities;

                if (parentID != InvalidEntityID)
                {
                    TransformHierarchyComponent* parentTransformComponent = data.ecs.get().GetComponent<TransformHierarchyComponent>(parentID);
                    ASSERT(parentTransformComponent != nullptr);
                    ReorderInternal(parentTransformComponent->children, data.oldIndex, data.newIndex);
                }
                else
                {
                    ReorderInternal(rootEntities, data.oldIndex, data.newIndex);
                }
            };

        auto undoCommand = [](const ReorderEntityData& data)
            {
                const EntityID parentID = GetParentEntity(data.ecs, data.entityID);
                auto& rootEntities = data.rootEntities;

                if (parentID != InvalidEntityID)
                {
                    TransformHierarchyComponent* parentTransformComponent = data.ecs.get().GetComponent<TransformHierarchyComponent>(parentID);
                    ASSERT(parentTransformComponent != nullptr);
                    ReorderInternal(parentTransformComponent->children, data.oldIndex, data.newIndex);
                }
                else
                {
                    ReorderInternal(rootEntities, data.newIndex, data.oldIndex);
                }
            };

        commandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Reorder Entity"));
    }

    void SetParentEntity(ECS& ecs, const EntityID parentID, const EntityID childID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker, ChildIndexSetting indexSetting = LastIndex{})
    {
        if (!IsEntityChildable(ecs, parentID, childID))
        {
            if (parentID != InvalidEntityID)
            {
                return;
            }
        }

        struct SetParentEntityData final
        {
            std::reference_wrapper<ECS> ecs;
            EntityID parentID;
            EntityID childID;
            EntityID oldParentID;
            std::reference_wrapper<std::vector<EntityID>> rootEntities;
            std::size_t childIndex = 0;
            ChildIndexSetting indexSetting;
        };

        const EntityID oldParent = GetParentEntity(ecs, childID);
        SetParentEntityData data
        {
            .ecs = ecs,
            .parentID = parentID,
            .childID = childID,
            .oldParentID = oldParent,
            .rootEntities = rootEntities,
            .indexSetting = indexSetting
        };

        if (data.oldParentID == data.parentID)
        {
            return;
        }

        if (oldParent == InvalidEntityID)
        {
            auto it = std::ranges::find(rootEntities, childID);
            if (it == end(rootEntities))
            {
                throw std::runtime_error("Child entity not found in root entities");
            }

            data.childIndex = std::ranges::distance(begin(rootEntities), it);
        }
        else
        {
            auto parentTransformComponent = ecs.GetComponent<TransformHierarchyComponent>(oldParent);
            auto it = std::ranges::find(parentTransformComponent->children, childID);
            if (it == end(parentTransformComponent->children))
            {
                throw std::runtime_error("Child entity not found in parent's children");
            }

            data.childIndex = std::ranges::distance(begin(parentTransformComponent->children), it);
        }

        auto doCommand = [](const SetParentEntityData& data)
            {
                SetParentEntity(data.ecs, data.childID, data.parentID, data.indexSetting);

                if (data.oldParentID == InvalidEntityID)
                {
                    data.rootEntities.get().erase(data.rootEntities.get().begin() + data.childIndex);
                }
                else if (data.parentID == InvalidEntityID)
                {
                    std::visit(Visitor
                        {
                            [&](const Index& index)
                            {
                                data.rootEntities.get().insert(begin(data.rootEntities.get()) + index.mIndex, data.childID);
                            },
                            [&](const FirstIndex&)
                            {
                                data.rootEntities.get().insert(data.rootEntities.get().begin(), data.childID);
                            },
                            [&](const LastIndex&)
                            {
                                data.rootEntities.get().push_back(data.childID);
                            }
                        }, data.indexSetting);
                }
            };

        auto undoCommand = [](const SetParentEntityData& data)
            {
                SetParentEntity(data.ecs, data.childID, data.oldParentID, Index{ data.childIndex });

                if (data.oldParentID == InvalidEntityID)
                {
                    data.rootEntities.get().insert(data.rootEntities.get().begin() + data.childIndex, data.childID);
                }
                else if (data.parentID == InvalidEntityID)
                {
                    std::visit(Visitor
                        {
                            [&](const Index& index)
                            {
                                data.rootEntities.get().erase(begin(data.rootEntities.get()) + index.mIndex);
                            },
                            [&](const FirstIndex&)
                            {
                                data.rootEntities.get().erase(data.rootEntities.get().begin());
                            },
                            [&](const LastIndex&)
                            {
                                data.rootEntities.get().pop_back();
                            }
                        }, data.indexSetting);
                }
            };

        commandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Add Child Entity"));
    }

    [[nodiscard]] EditorAction CreateSetParentEntityAction(ECS& ecs, const EntityID parentID, const EntityID childID, std::vector<EntityID>& rootEntities)
    {
        return [&ecs, parentID, childID, &rootEntities](EditorCommandTracker& commandTracker)
            {
                SetParentEntity(ecs, parentID, childID, rootEntities, commandTracker);
            };
    }


    [[nodiscard]] void OnEntityDroppedOnEntity(ECS& ecs, const EntityID parentID, const EntityID childID, std::vector<EntityID>& rootEntities, std::set<EntityID>& selectedEntityIDs, EditorCommandTracker& commandTracker)
    {
        commandTracker.BeginComposite("Set Parent Entity + Select Entity");
        SetParentEntity(ecs, parentID, childID, rootEntities, commandTracker);
        SetEntitySelection(childID, selectedEntityIDs, commandTracker);
        commandTracker.EndComposite();
    }

    [[nodiscard]] EditorAction CreateEntityDroppedOnEntityAction(ECS& ecs, const EntityID parentID, const EntityID childID, std::vector<EntityID>& rootEntities, std::set<EntityID>& selectedEntityIDs)
    {
        return [&ecs, parentID, childID, &rootEntities, &selectedEntityIDs](EditorCommandTracker& commandTracker)
            {
                OnEntityDroppedOnEntity(ecs, parentID, childID, rootEntities, selectedEntityIDs, commandTracker);
            };
    }

    [[nodiscard]] static std::vector<EditorAction> ShowEntityPayload(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, std::set<EntityID>& selectedEntityIDs)
    {
        ObjectSource(entityID, "EntityID");

        if (const std::optional<EntityID> childEntityID = ObjectTarget<EntityID>())
        {
            return
            {
                CreateEntityDroppedOnEntityAction(ecs, entityID, childEntityID.value(), rootEntities, selectedEntityIDs)
            };
        }
        return {};
    }

    static ImGuiTreeNodeFlags GetHierarchyTreeNodeFlags(const bool isSelected, const bool isLeaf, const bool shouldBeOpen)
    {
        ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
        treeNodeFlags |= isSelected ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
        treeNodeFlags |= isLeaf ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_None;
        treeNodeFlags |= shouldBeOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None;
        treeNodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth;
        return treeNodeFlags;
    }

    [[nodiscard]] static std::vector<EditorAction> ShowEntityOptionsPopUp(const std::string& entityOptionsPopUpName, ECS& ecs, const EntityID entityID, std::set<EntityID>& selectedEntities,
        std::vector<EntityID>& rootEntities, const DataTypeRegistry& dataTypeRegistry, const std::string& imGuiTag)
    {
        std::vector<EditorAction> editorActions;
        if (ImGui::BeginPopup(entityOptionsPopUpName.c_str()))
        {

            if (ImGui::MenuItem(("Create Child Entity" + imGuiTag).c_str()))
            {
                editorActions.push_back([&ecs, entityID, &selectedEntities](EditorCommandTracker& commandTracker)
                    {
                        commandTracker.BeginComposite("Create Child Entity Composite");
                        const EntityID newEntityID = CreateChildEntity(ecs, entityID, commandTracker);
                        SetEntitySelection({ newEntityID }, selectedEntities, commandTracker);
                        commandTracker.EndComposite();
                    });
            }

            const std::string removeItem = "Remove" + imGuiTag;
            if (ImGui::MenuItem(removeItem.c_str()))
            {
                editorActions.push_back([&ecs, entityID, &selectedEntities, &rootEntities](EditorCommandTracker& commandTracker)
                    {
                        commandTracker.BeginComposite("Destroy Entity Composite");
                        DestroyEntity(ecs, entityID, rootEntities, commandTracker);
                        ClearEntitySelection(selectedEntities, commandTracker);
                        commandTracker.EndComposite();
                    });
            }

            std::string moveUpString = "Move Up" + imGuiTag;
            const EntityID parentID = GetParentEntity(ecs, entityID);
            ImGui::BeginDisabled(parentID == InvalidEntityID);
            if (ImGui::MenuItem(moveUpString.c_str()))
            {
                const EntityID parentOfParentID = GetParentEntity(ecs, parentID);

                const std::size_t insertionIndex = GetEntityIndexInParent(ecs, parentID, rootEntities);

                editorActions.push_back([&ecs, entityID, parentOfParentID, &rootEntities, insertionIndex](EditorCommandTracker& commandTracker)
                    {
                        SetParentEntity(ecs, parentOfParentID, entityID, rootEntities, commandTracker, Index{ insertionIndex });
                    });
            }
            ImGui::EndDisabled();

            if (ImGui::MenuItem(("Duplicate" + imGuiTag).c_str()))
            {
                editorActions.push_back([&ecs, entityID, &rootEntities, &selectedEntities, &dataTypeRegistry](EditorCommandTracker& commandTracker)
                    {
                        commandTracker.BeginComposite("Duplicate Entity Composite");
                        const EntityID newEntityID = DuplicateEntityAndChildren(ecs, entityID, rootEntities, dataTypeRegistry, commandTracker);
                        SetEntitySelection({ newEntityID }, selectedEntities, commandTracker);
                        commandTracker.EndComposite();
                    });
            }

            ImGui::EndPopup();
        }

        return editorActions;
    }


    static void OnEntityDroppedBetween(ECS& ecs, const EntityID droppedEntityID, const EntityID droppedBeforeEntityID, std::vector<EntityID>& rootEntities, std::set<EntityID>& selectedEntityIDs, const bool droppedAfter, EditorCommandTracker& commandTracker)
    {
        ASSERT(droppedEntityID != InvalidEntityID);
        ASSERT(droppedBeforeEntityID != InvalidEntityID);
        if (droppedEntityID == droppedBeforeEntityID)
        {
            return;
        }

        const std::size_t insertionIndex = GetEntityIndexInParent(ecs, droppedBeforeEntityID, rootEntities);

        const EntityID parentID = GetParentEntity(ecs, droppedBeforeEntityID);
        const EntityID currentParentID = GetParentEntity(ecs, droppedEntityID);

        commandTracker.BeginComposite("Move Entity + Select Entity");
        if (parentID == currentParentID)
        {
            ReorderEntity(ecs, droppedEntityID, rootEntities, insertionIndex + (droppedAfter ? 1 : 0), commandTracker);
        }
        else
        {
            SetParentEntity(ecs, parentID, droppedEntityID, rootEntities, commandTracker, Index{ insertionIndex + (droppedAfter ? 1 : 0) });
        }

        SetEntitySelection(droppedEntityID, selectedEntityIDs, commandTracker);
        commandTracker.EndComposite();
    }

    [[nodiscard]] EditorAction CreateEntityDropAction(ECS& ecs, const EntityID droppedEntityID, const EntityID droppedBeforeEntityID, bool droppedAfter, std::vector<EntityID>& rootEntities, std::set<EntityID>& selectedEntityIDs)
    {
        return [&ecs, &rootEntities, droppedEntityID, droppedBeforeEntityID, &selectedEntityIDs, droppedAfter](EditorCommandTracker& commandTracker)
            {
                OnEntityDroppedBetween(ecs, droppedEntityID, droppedBeforeEntityID, rootEntities, selectedEntityIDs, droppedAfter, commandTracker);
            };
    }

    [[nodiscard]] static std::vector<EditorAction> ShowEntityDropSpaceTarget(ECS& ecs, const EntityID droppedBesideEntityID, const bool droppedAfter, std::vector<EntityID>& rootEntities, std::set<EntityID>& selectedEntityIDs)
    {
        std::vector<EditorAction> editorActions;
        ImGui::PushID(droppedBesideEntityID.id);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
        ImGui::InvisibleButton(std::to_string(droppedBesideEntityID.id).c_str(), ImVec2{ ImGui::GetContentRegionAvail().x, 3 });
        ImGui::PopStyleVar();
        if (std::optional<EntityID> droppedEntityID = ObjectTarget<EntityID>())
        {
            editorActions.push_back(CreateEntityDropAction(ecs, droppedEntityID.value(), droppedBesideEntityID, droppedAfter, rootEntities, selectedEntityIDs));
        }
        ImGui::PopID();

        return editorActions;
    }

    [[nodiscard]] static const std::string& GetEntityName(const ECS& ecs, const EntityID entityID)
    {
        const NameComponent* nameComponent = ecs.GetComponent<NameComponent>(entityID);
        ASSERT(nameComponent != nullptr);
        return nameComponent->name;
    }

    [[nodiscard]] EditorAction CreateSetEntitySelectionAction(const EntityID entityID, std::set<EntityID>& selectedEntityIDs)
    {
        return [entityID, &selectedEntityIDs](EditorCommandTracker& commandTracker)
            {
                SetEntitySelection(entityID, selectedEntityIDs, commandTracker);
            };
    }

    template<typename T>
    EntityID FindFirstParentEntityWithComponent(const ECS& ecs, const EntityID childEntityID, const bool includeChild = true)
    {
        ASSERT(childEntityID != InvalidEntityID);
        EntityID check = includeChild ? childEntityID : GetParentEntity(ecs, childEntityID);
        while (check != InvalidEntityID)
        {
            if (ecs.HasComponent<T>(check))
            {
                return check;
            }
            check = GetParentEntity(ecs, check);
        }
        return InvalidEntityID;
    }

    [[nodiscard]] static std::vector<EditorAction> ShowEntityChildren(ECS& ecs, const EntityID entityID, std::set<EntityID>& selectedEntityIDs, ECS& ecsBuffer,
        std::vector<EntityID>& rootEntities, const std::span<const EntityID> parentEntities, const std::string& imGuiTag, const std::set<EntityID>& uneditableEntities, 
        const DataTypeRegistry& dataTypeRegistry, const std::function<bool(EntityID)>& filter)
    {
        std::vector<EditorAction> editorActions;
        const bool isSelected = selectedEntityIDs.contains(entityID);
        const bool isLeaf = GetEntityChildren(ecs, entityID).empty();
        const bool shouldBeDefaultOpen = std::ranges::find(parentEntities, entityID) != end(parentEntities);

        const bool isUneditable = uneditableEntities.contains(entityID);
        InsertRange(editorActions, ShowEntityDropSpaceTarget(ecs, entityID, false, rootEntities, selectedEntityIDs));
        ImGui::PushID(entityID.id);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 3 });
        const bool isOpen = ImGui::TreeNodeEx(GetEntityName(ecs, entityID).c_str(), GetHierarchyTreeNodeFlags(isSelected, isLeaf, shouldBeDefaultOpen));
        ImGui::PopStyleVar();
        ImGui::PopID();

        const bool isLeftClicked = ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left);
        const bool isRightClicked = ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right);
        if (isRightClicked)
        {
            editorActions.push_back(CreateSetEntitySelectionAction(entityID, selectedEntityIDs));
        }

        if (selectedEntityIDs.contains(entityID))
        {
            //ImGui::SetItemDefaultFocus();

            const std::string entityOptionsPopUpName = "Entity Settings" + imGuiTag;
            if (isRightClicked && !isUneditable)
            {
                ImGui::OpenPopup(entityOptionsPopUpName.c_str());
            }

            InsertRange(editorActions, ShowEntityOptionsPopUp(entityOptionsPopUpName, ecs, entityID, selectedEntityIDs, rootEntities, dataTypeRegistry, imGuiTag));
        }

        InsertRange(editorActions, ShowEntityPayload(ecs, entityID, rootEntities, selectedEntityIDs));

        if (isLeftClicked && !ImGui::IsItemToggledOpen())
        {
            editorActions.push_back(CreateSetEntitySelectionAction(entityID, selectedEntityIDs));
        }

        if (isOpen)
        {
            auto children = GetEntityChildren(ecs, entityID);
            for (const EntityID childEntityID : children)
            {
                ASSERT(childEntityID != InvalidEntityID);
                if (!filter(childEntityID))
                {
                    continue;
                }
                InsertRange(editorActions, ShowEntityChildren(ecs, childEntityID, selectedEntityIDs, ecsBuffer, rootEntities, parentEntities, imGuiTag, uneditableEntities, dataTypeRegistry, filter));
            }

            if (!children.empty())
            {
                InsertRange(editorActions, ShowEntityDropSpaceTarget(ecs, children.back(), true, rootEntities, selectedEntityIDs));
            }

            ImGui::TreePop();
        }

        return editorActions;
    }


    void ShowEntityHierarchy(ECS& ecs, ECS& ecsBuffer, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker,
        const std::string& imGuiTag, std::set<EntityID>& selectedEntityIDs, const std::set<EntityID>& uneditableEntities, std::string& entitySearchBuffer, const DataTypeRegistry& dataTypeRegistry)
    {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColor(0.18f, 0.18f, 0.18f, 0.80f).Value);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(0.12f, 0.12f, 0.12f, 0.0f).Value);
        ImGui::PushStyleColor(ImGuiCol_Border, ImColor(0.12f, 0.12f, 0.12f, 0.0f).Value);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

        auto filter = [&](const EntityID entityID)
            {
                if (entitySearchBuffer.empty())
                {
                    return true;
                }
                auto nameSearch = [&](const EntityID childID)
                    {
                        return CaseInsensitiveContains(GetEntityName(ecs, childID), entitySearchBuffer);
                    };
                if (nameSearch(entityID))
                {
                    return true;
                }
                auto allEntityChildren = GetAllEntityChildren(ecs, entityID);
                return std::ranges::any_of(allEntityChildren, nameSearch);
            };

        const std::string listBoxName = "" + imGuiTag;

        const ImVec2 parentSize = ImGui::GetContentRegionAvail();

        std::vector<EditorAction> editorActions;
        if (ImGui::BeginListBox(listBoxName.c_str(), parentSize))
        {
            std::vector<EntityID> parentEntites;
            if (!selectedEntityIDs.empty())
            {
                for (const EntityID selectedEntityID : selectedEntityIDs)
                {
                    if (selectedEntityID != InvalidEntityID)
                    {
                        InsertRange(parentEntites, GetParents(ecs, selectedEntityID));
                    }
                }
            }
            for (const EntityID rootEntityID : rootEntities)
            {
                if (!filter(rootEntityID))
                {
                    continue;
                }
                InsertRange(editorActions, ShowEntityChildren(
                    ecs,
                    rootEntityID,
                    selectedEntityIDs,
                    ecsBuffer,
                    rootEntities,
                    parentEntites,
                    imGuiTag,
                    uneditableEntities,
                    dataTypeRegistry,
                    filter
                ));
            }

            if (!rootEntities.empty())
            {
                InsertRange(editorActions, ShowEntityDropSpaceTarget(ecs, rootEntities.back(), true, rootEntities, selectedEntityIDs));
            }

            ImGui::EndListBox();
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();

        for (auto& action : editorActions)
        {
            action(commandTracker);
        }
    }

    static void ShowEntitySearchBar(std::string& entitySearchString)
    {
        char searchBufferArray[256]{};

        CopyString(searchBufferArray, entitySearchString);
        if (ImGui::InputTextWithHint("##EntitySearch", "Search...", &searchBufferArray[0], sizeof(searchBufferArray)))
        {
            entitySearchString = searchBufferArray;
        }
    }

    void ShowEntityHierarchyWithAddButtons(ECS& ecs, ECS& ecsBuffer, std::vector<EntityID>& rootEntities,
        EditorCommandTracker& commandTracker, const std::string& imGuiTag, std::set<EntityID>& selectedEntityIDs, const EntityID defaultParent, 
        const std::set<EntityID>& uneditableEntities, std::string& entitySearchBuffer, const DataTypeRegistry& dataTypeRegistry)
    {
        ShowEntityAddButtons(ecs, selectedEntityIDs, rootEntities, commandTracker, imGuiTag, defaultParent);
        ImGui::Separator();
        ShowEntitySearchBar(entitySearchBuffer);
        ImGui::Separator();
        ShowEntityHierarchy(ecs, ecsBuffer, rootEntities, commandTracker, imGuiTag, selectedEntityIDs, uneditableEntities, entitySearchBuffer, dataTypeRegistry);
    }

    [[nodiscard]] static EditorAction CreateRemoveComponentAction(ECS& ecs, const EntityID entityID, const DataTypeID& dataTypeID, ECS& ecsBuffer)
    {
        return [&ecs, entityID, dataTypeID, &ecsBuffer](EditorCommandTracker& commandTracker)
            {
                RemoveComponent(ecs, entityID, dataTypeID, ecsBuffer, commandTracker);
            };
    }

    [[nodiscard]] static std::optional<EditorAction> ShowComponentData(ECS& ecs, const EntityID entityID, const std::type_info& typeInfo, void* componentPtr, bool& anyActiveItem, ECS& ecsBuffer, 
        JsonAny& copiedComponent, const DataTypeRegistry& dataTypeRegistry, const Blackboard& blackboard)
    {
        PROFILER_FUNCTION(profiler::colors::Lime400);

        ASSERT(componentPtr != nullptr);
        ImGui::AlignTextToFramePadding();

        const DataTypeID componentDataTypeID = GetDataTypeID(typeInfo);

        const DataType* dataType = dataTypeRegistry.Find(componentDataTypeID);
        if (dataType == nullptr)
        {
            return std::nullopt;
        }

        Blackboard newBlackboard = blackboard;
        PropertyPath propertyPath;
        propertyPath.entityID = entityID;
        propertyPath.componentDataTypeID = componentDataTypeID;
        newBlackboard.Insert<Key_CurrentPropertyPath>(propertyPath);

        const std::string& componentName = dataType->prettyName + "##Component";

        const bool isOpen = ImGui::TreeNodeEx(componentName.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);

        ImGui::SameLine(ImGui::GetWindowWidth() - 50);

        if (ImGui::Button("..."))
        {
            ImGui::OpenPopup("Component Options");
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("More Options");
        }

        if (isOpen)
        {
            PROFILER_BEGIN("View Component Data");
            const ViewAndEditResult viewAndEditResult = dataTypeRegistry.ViewAndEditData(componentDataTypeID, componentPtr, newBlackboard);
            anyActiveItem |= viewAndEditResult.isActive;
            PROFILER_END();
        }

        std::optional<EditorAction> removeComponentAction;
        if (ImGui::BeginPopup("Component Options"))
        {
            ImGui::BeginDisabled(ecs.GetRegistry().GetComponentType(GetDataTypeID(typeInfo)).isDefault);
            if (ImGui::MenuItem("Remove"))
            {
                removeComponentAction = CreateRemoveComponentAction(ecs, entityID, GetDataTypeID(typeInfo), ecsBuffer);
            }
            ImGui::EndDisabled();

            if (ImGui::MenuItem("Copy"))
            {
                copiedComponent.dataTypeID = componentDataTypeID;
                copiedComponent.json = dataTypeRegistry.SaveDataJSON(componentDataTypeID, componentPtr);
            }

            ImGui::BeginDisabled(copiedComponent.json.is_null() || copiedComponent.dataTypeID != componentDataTypeID);
            if (ImGui::MenuItem("Paste"))
            {
                dataTypeRegistry.LoadDataJSON(componentDataTypeID, componentPtr, copiedComponent.json, newBlackboard);
            }
            ImGui::EndDisabled();

            ImGui::EndPopup();
        }

        if (isOpen)
        {
            ImGui::TreePop();
        }

        ImGui::Separator();

        return removeComponentAction;
    }

    void SetEntityName(ECS& ecs, const EntityID entityID, const std::string& newName, EditorCommandTracker& commandTracker)
    {
        struct SetNameData final
        {
            std::reference_wrapper<ECS> ecs;
            EntityID entityID;
            std::string newName;
            std::string oldName;
        };

        std::string oldName = GetEntityName(ecs, entityID);

        SetNameData data
        {
            .ecs = ecs,
            .entityID = entityID,
            .newName = newName,
            .oldName = std::move(oldName)
        };

        auto doCommand = [](const SetNameData& data)
            {
                NameComponent* nameComponent = data.ecs.get().GetComponent<NameComponent>(data.entityID);
                ASSERT(nameComponent != nullptr);
                nameComponent->name = data.newName;
            };

        auto undoCommand = [](const SetNameData& data)
            {
                NameComponent* nameComponent = data.ecs.get().GetComponent<NameComponent>(data.entityID);
                ASSERT(nameComponent != nullptr);
                nameComponent->name = data.oldName;
            };

        commandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Set Entity Name"));
    }

    [[nodiscard]] static EditorAction CreateSetEntityNameAction(ECS& ecs, const EntityID entityID, const std::string& newName)
    {
        return [&ecs, entityID, newName](EditorCommandTracker& commandTracker)
            {
                SetEntityName(ecs, entityID, newName, commandTracker);
            };
    }

    [[nodiscard]] std::optional<EditorAction> ShowEntityName(ECS& ecs, const EntityID selectedEntityID, const InputState& input)
    {
        if (selectedEntityID == InvalidEntityID)
        {
            return std::nullopt;
        }
        NameComponent* nameComponent = ecs.GetComponent<NameComponent>(selectedEntityID);
        ASSERT(nameComponent != nullptr);
        std::string& selectedEntityName = nameComponent->name;

        char nameBuffer[256]{};
        CopyString(nameBuffer, selectedEntityName);

        ImGui::PushItemWidth(200);

        std::optional<EditorAction> setNameAction;
        if (ImGui::InputTextWithHint("Name", "Entity Name", nameBuffer, sizeof(nameBuffer)))
        {
            if (input.IsKeyPressed(eInputKey::Enter))
            {
                setNameAction = CreateSetEntityNameAction(ecs, selectedEntityID, nameBuffer);
            }
        }

        ImGui::PopItemWidth();

        ImGui::SameLine(ImGui::GetWindowWidth() - 70);
        ImGui::Text(std::string("ID: " + std::to_string(selectedEntityID.id)).c_str());
        ImGui::Separator();

        return setNameAction;
    }


    void ChangeComponentValue(ECS& ecs, ECS& ecsBuffer, const EntityID entityID, const EntityID copyEntityID, EditorCommandTracker& commandTracker)
    {

        struct ChangeComponentValueData final
        {
            std::reference_wrapper<ECS> ecs;
            std::reference_wrapper<ECS> ecsBuffer;
            EntityID entityID;
            EntityID copyEntityID;
        };

        ChangeComponentValueData data
        {
            .ecs = ecs,
            .ecsBuffer = ecsBuffer,
            .entityID = entityID,
            .copyEntityID = copyEntityID
        };

        auto command = [](const ChangeComponentValueData& data)
            {
                SwapEntities(data.ecs, data.ecsBuffer, data.entityID, data.copyEntityID);
            };

        commandTracker.RegisterCommand(EditorCommand(data, command, command, "Change Component Value"));
    }

    [[nodiscard]] static EditorAction CreateChangeComponentValueAction(ECS& ecs, ECS& ecsBuffer, const EntityID entityID, const EntityID copyEntityID)
    {
        return [&ecs, &ecsBuffer, entityID, copyEntityID](EditorCommandTracker& commandTracker)
            {
                ChangeComponentValue(ecs, ecsBuffer, entityID, copyEntityID, commandTracker);
            };
    }

    [[nodiscard]] bool ShouldHideComponent(const std::type_info& typeInfo)
    {
        return (typeInfo == typeid(NameComponent) || typeInfo == typeid(TransformHierarchyComponent)) || typeInfo == typeid(EditorComponent);
    }

    [[nodiscard]] std::vector<EditorAction> ShowEntityComponents(ECS& ecs, const EntityID selectedEntityID, bool& anyItemActiveLastFrame,
        ECS& ecsBuffer, EntityID& copyEntityID, JsonAny& copiedComponent, const Blackboard& blackboard)
    {
        PROFILER_FUNCTION(profiler::colors::Brown400);

        if (!anyItemActiveLastFrame)
        {
            copyEntityID = ecs.CopyEntity(selectedEntityID, ecsBuffer);
        }

        bool anyActiveItem = false;

        std::vector<EditorAction> editorActions;

        static bool showAllComponents = false;
        for (auto [typeInfo, componentPtr] : ecs.ViewEntity(selectedEntityID))
        {
            bool isHiddenComponent = ShouldHideComponent(typeInfo);
            if (isHiddenComponent)
            {
                if (showAllComponents)
                {
                    ImGui::BeginDisabled(true);
                }
                else
                {
                    continue;
                }
            }
            auto editorAction = ShowComponentData(
                ecs, 
                selectedEntityID, 
                typeInfo, 
                componentPtr,
                anyActiveItem, 
                ecsBuffer, 
                copiedComponent, 
                blackboard.Get<Key_DataTypeRegistry>(), 
                blackboard
            );
            if (editorAction)
            {
                editorActions.push_back(std::move(editorAction.value()));
            }
            if (isHiddenComponent && showAllComponents)
            {
                ImGui::EndDisabled();
            }
        }

        if (!anyActiveItem && anyItemActiveLastFrame)
        {
            auto action = CreateChangeComponentValueAction(ecs, ecsBuffer, selectedEntityID, copyEntityID);
            if (action)
            {
                editorActions.push_back(std::move(action));
            }

            copyEntityID = InvalidEntityID;
        }

        if (!anyActiveItem && !anyItemActiveLastFrame)
        {
            ecsBuffer.DestroyEntity(copyEntityID);
        }

        anyItemActiveLastFrame = anyActiveItem;

        return editorActions;
    }

    [[nodiscard]] std::optional<EditorAction> ShowEntityAddComponentButtons(ECS& ecs, const EntityID entityID, uint32_t& selectedIndex, std::string& searchString, const DataTypeRegistry& dataTypeRegistry)
    {
        const std::string popupID = "Add Component##" + std::to_string(entityID.id);

        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup(popupID.c_str());
        }

        if (ImGui::BeginPopup(popupID.c_str()) == false)
        {
            selectedIndex = 0;
            return std::nullopt;
        }
        else
        {
            searchString = "";
        }

        char searchBuffer[256]{};

        CopyString(searchBuffer, searchString);

        if (ImGui::InputTextWithHint("Search##Component", "Search", searchBuffer, IM_ARRAYSIZE(searchBuffer), ImGuiInputTextFlags_AutoSelectAll))
        {
            selectedIndex = 0;
            searchString = searchBuffer;
        }

        auto nameMatchesInput = [&searchString](const DataType& dataType)
            {
                return searchString.empty() || CaseInsensitiveContains(dataType.prettyName, searchString);
            };

        auto isValidComponentDataType = [&ecs, entityID](const DataType& dataType)
            {
                return dataType.isComponent
                    && !ecs.GetRegistry().GetComponentType(GetDataTypeID(dataType.typeInfo)).isDefault
                    && !ecs.HasComponent(entityID, GetDataTypeID(dataType.typeInfo));
            };

        auto componentDataTypes = dataTypeRegistry.GetDataTypesFiltered(isValidComponentDataType)
            | std::views::values
            | std::views::filter(nameMatchesInput)
            | std::ranges::to<std::vector>();

        std::ranges::sort(componentDataTypes, [](const auto& a, const auto& b) { return a.prettyName < b.prettyName; });

        std::optional<EditorAction> addComponentAction;
        uint32_t indexCounter = 0;
        for (const auto& dataType : componentDataTypes)
        {
            const std::string componentNameLabel = dataType.prettyName + "##Component";
            if (ImGui::Selectable(componentNameLabel.c_str(), selectedIndex == indexCounter))
            {
                addComponentAction = CreateAddComponentToEntityAction(ecs, entityID, GetDataTypeID(dataType.typeInfo), dataType.prettyName);
                selectedIndex = 0;
                ImGui::CloseCurrentPopup();
            }

            indexCounter++;
        }

        if (!addComponentAction)
        {
            if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
            {
                selectedIndex = (selectedIndex == 0) ? static_cast<uint32_t>(componentDataTypes.size() - 1) : selectedIndex - 1;
            }
            else if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
            {
                selectedIndex = (selectedIndex + 1) % static_cast<uint32_t>(componentDataTypes.size());
            }
            else if (ImGui::IsKeyPressed(ImGuiKey_Enter) && !componentDataTypes.empty())
            {
                const auto& dataType = componentDataTypes[selectedIndex];
                addComponentAction = CreateAddComponentToEntityAction(ecs, entityID, GetDataTypeID(dataType.typeInfo), dataType.prettyName);
                selectedIndex = 0;
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();

        return addComponentAction;
    }

    [[nodiscard]] std::vector<EditorAction> ShowEntityInspector(ECS& ecs, const EntityID entityID, bool& anyItemActiveLastFrame,
        ECS& ecsBuffer, EntityID& copyEntityID, uint32_t& selectedIndex, std::string& componentSearchString, JsonAny& copiedComponent, const Blackboard& blackboard)
    {
        PROFILER_FUNCTION(profiler::colors::Pink200);
        if (entityID == InvalidEntityID)
        {
            return {};
        }
        std::vector<EditorAction> editorActions;
        auto showEntityComponents = ShowEntityComponents(ecs, entityID, anyItemActiveLastFrame, ecsBuffer, copyEntityID, copiedComponent, blackboard);
        InsertRange(editorActions, showEntityComponents);
        if (auto action = ShowEntityAddComponentButtons(ecs, entityID, selectedIndex, componentSearchString, blackboard.Get<Key_DataTypeRegistry>()))
        {
            editorActions.push_back(*action);
        }
        return editorActions;
    }

    EntityIDConverter MergeECS(ECS& targetECS, const ECS& sourceECS)
    {
        auto entityCollectionView = sourceECS.ViewEntities();
        std::vector<EntityID> entityConverter(entityCollectionView.GetCount());
        for (auto entity : entityCollectionView)
        {
            const EntityID createdEntityID = sourceECS.CopyEntity(entity.GetEntityID(), targetECS);
            entityConverter[entity.GetEntityID().id] = createdEntityID;
        }
        return EntityIDConverter(std::move(entityConverter));
    }

    EntityID InstantiateEntityComposition(ECS& targetECS, const EntityCompositionAssetHandle& compositionAsset, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
    {
        ASSERT(compositionAsset.IsValid());
        commandTracker;
        const EntityComposition& entityComposition = compositionAsset.Get();
        const EntityIDConverter entityConverter = MergeECS(targetECS, entityComposition.GetECS());
        UpdateEntityIDs(entityComposition.GetECS(), targetECS, entityConverter);
        const EntityID instantiatedRootEntity = entityConverter[entityComposition.GetRootEntity()];
        targetECS.AddComponent<EntityCompositionComponent>(instantiatedRootEntity).asset = compositionAsset;
        rootEntities = GetRootEntities(targetECS);
        return instantiatedRootEntity;
    }

    void TeleportCameraToEntity(const ECS& ecs, const EntityID entityID, Camera& camera, const bool changeRotation, const float offsetDistance)
    {
        if (entityID == InvalidEntityID)
        {
            return;
        }
        const Point3f entityPos = GetWorldTransform(ecs, entityID).GetPosition();
        const Point3f oldCamPos = camera.GetTransform().GetPosition();
        if (entityPos == oldCamPos)
        {
            return;
        }
        if (changeRotation)
        {
            const UnitVector3f camForward = GetUnitVector(oldCamPos, entityPos);
            const UnitVector3f camRight = Cross(UnitVector3f::Up(), camForward);
            const RotationMatrix3f matrix = RotationMatrix3f::FromXZ(camRight, camForward);
            camera.GetTransform().SetRotation(matrix);
            const Point3f newCamPos = entityPos + (-camForward) * offsetDistance;
            camera.SetPosition(newCamPos);
        }
        else
        {
            const Point3f newCamPos = entityPos + (-camera.GetForward()) * offsetDistance;
            camera.SetPosition(newCamPos);
        }
    }
}