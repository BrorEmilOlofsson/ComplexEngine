#include "Editor/Precompiled/EditorPch.hpp"
#include "ECSEditorFunctions.hpp"
#include "Editor/Command/Core/EditorCommandTracker.hpp"
#include "Engine/ECSEngine/Components/NameComponent.hpp"
#include "Engine/ECSEngine/Components/TransformHierarchyComponent.hpp"
#include "Editor/Command/Commands/SetDataPtrCommand.hpp"
#include "Engine/ECSEngine/Components/MeshComponent.hpp"
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

    static EntityID CreateRootEntity(ECS& ecs, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
    {
        const EntityID createdEntityID = ecs.CreateEntity();

        struct CreateEntityData final
        {
            EntityID mCreatedEntityID;
            ECS* mECS = nullptr;
            std::vector<EntityID>* mRootEntities = nullptr;
            std::size_t mRootIndex = 0;
        } data;

        data.mCreatedEntityID = createdEntityID;
        data.mECS = &ecs;
        data.mRootEntities = &rootEntities;
        data.mRootIndex = rootEntities.size();

        auto doCommand = [](const CreateEntityData& data)
            {
                data.mRootEntities->insert(begin(*data.mRootEntities) + data.mRootIndex, data.mCreatedEntityID);
                ECS::EntityView(data.mECS, data.mCreatedEntityID).SetIsActive(true);
            };

        auto undoCommand = [](const CreateEntityData& data)
            {
                data.mRootEntities->erase(begin(*data.mRootEntities) + data.mRootIndex);
                ECS::EntityView(data.mECS, data.mCreatedEntityID).SetIsActive(false);
            };

        commandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Create Root Entity"));

        return createdEntityID;
    }

    EntityID CreateChildEntity(ECS& ecs, const EntityID parentID, EditorCommandTracker& commandTracker)
    {
        const EntityID createdEntityID = ecs.CreateEntity();
        struct CreateEntityData final
        {
            EntityID mCreatedEntityID;
            std::reference_wrapper<ECS> mECS;
            EntityID mParentID;
            std::size_t mIndex = 0;
        };

        CreateEntityData data
        {
            .mCreatedEntityID = createdEntityID,
            .mECS = ecs,
            .mParentID = parentID,
            .mIndex = ecs.GetComponent<TransformHierarchyComponent>(parentID)->children.size()
        };

        auto doCommand = [](const CreateEntityData& data)
            {
                TransformHierarchyComponent* parentComponent = data.mECS.get().GetComponent<TransformHierarchyComponent>(data.mParentID);
                parentComponent->children.insert(begin(parentComponent->children) + data.mIndex, data.mCreatedEntityID);
                TransformHierarchyComponent* childComponent = data.mECS.get().GetComponent<TransformHierarchyComponent>(data.mCreatedEntityID);
                childComponent->parent = data.mParentID;
                ECS::EntityView(&data.mECS.get(), data.mCreatedEntityID).SetIsActive(true);
            };

        auto undoCommand = [](const CreateEntityData& data)
            {
                TransformHierarchyComponent* parentComponent = data.mECS.get().GetComponent<TransformHierarchyComponent>(data.mParentID);
                parentComponent->children.erase(begin(parentComponent->children) + data.mIndex);
                TransformHierarchyComponent* childComponent = data.mECS.get().GetComponent<TransformHierarchyComponent>(data.mCreatedEntityID);
                childComponent->parent = InvalidEntityID;
                ECS::EntityView(&data.mECS.get(), data.mCreatedEntityID).SetIsActive(false);
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

    static void DestroyRootEntity(const EntityID entityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
    {
        struct DestroyEntityData
        {
            EntityID mEntityID;
            std::size_t mIndex = 0;
            std::vector<EntityID>* mRootEntities = nullptr;
        } data;

        data.mEntityID = entityID;
        data.mIndex = std::ranges::distance(begin(rootEntities), std::ranges::find(rootEntities, entityID));
        data.mRootEntities = &rootEntities;

        auto doCommand = [](const DestroyEntityData& data)
            {
                data.mRootEntities->erase(begin(*data.mRootEntities) + data.mIndex);
            };

        auto undoCommand = [](const DestroyEntityData& aData)
            {
                aData.mRootEntities->insert(begin(*aData.mRootEntities) + aData.mIndex, aData.mEntityID);
            };

        commandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Destroy Root Entity"));
    }

    static void DestroyChildEntity(ECS& ecs, const EntityID entityID, EditorCommandTracker& aCommandTracker)
    {
        TransformHierarchyComponent* hierarchyComponent = ecs.GetComponent<TransformHierarchyComponent>(entityID);

        if (hierarchyComponent == nullptr)
        {
            assert(false && "Entity has no children transform component");
        }

        if (hierarchyComponent->parent == InvalidEntityID)
        {
            assert(false && "Parent not set");
        }


        struct DestroyEntityData final
        {
            EntityID mParentID;
            EntityID mChildID;
            std::size_t mIndex = 0;
            std::reference_wrapper<ECS> mECS;
        };

        TransformHierarchyComponent* parentChildrenTransformComponent = ecs.GetComponent<TransformHierarchyComponent>(hierarchyComponent->parent);

        DestroyEntityData data
        {
            .mParentID = hierarchyComponent->parent,
            .mChildID = entityID,
            .mIndex = static_cast<std::size_t>(std::ranges::distance(begin(parentChildrenTransformComponent->children), std::ranges::find(parentChildrenTransformComponent->children, entityID))),
            .mECS = ecs
        };

        auto doCommand = [](const DestroyEntityData& data)
            {
                TransformHierarchyComponent* parentChildrenTransformComponent = data.mECS.get().GetComponent<TransformHierarchyComponent>(data.mParentID);
                std::erase(parentChildrenTransformComponent->children, data.mChildID);
                ECS::EntityView(&data.mECS.get(), data.mChildID).SetIsActive(false);
            };

        auto undoCommand = [](const DestroyEntityData& data)
            {
                TransformHierarchyComponent* parentChildrenTransformComponent = data.mECS.get().GetComponent<TransformHierarchyComponent>(data.mParentID);
                parentChildrenTransformComponent->children.insert(begin(parentChildrenTransformComponent->children) + data.mIndex, data.mChildID);
                ECS::EntityView(&data.mECS.get(), data.mChildID).SetIsActive(true);
            };

        aCommandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Destroy Child Entity"));
    }

    void DestroyEntity(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
    {
        if (std::ranges::find(rootEntities, entityID) != end(rootEntities))
        {
            DestroyRootEntity(entityID, rootEntities, commandTracker);
        }
        else
        {
            DestroyChildEntity(ecs, entityID, commandTracker);
        }
    }

    void SelectEntity(EntityID entityID, EntityID& selectedEntityID, EditorCommandTracker& commandTracker)
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
    }

    static void AddComponentToEntity(ECS& ecs, const EntityID entityID, const std::type_index typeIndex, const std::string& componentName/*, ECS& ecsBuffer*/, EditorCommandTracker& commandTracker)
    {
        struct AddComponentData final
        {
            ECS* mECS = nullptr;
            EntityID mEntityID;
            std::type_index mTypeIndex;
        };

        AddComponentData data
        {
            .mECS = &ecs,
            .mEntityID = entityID,
            .mTypeIndex = typeIndex
        };

        auto doCommand = [](const AddComponentData& data)
            {
                data.mECS->GetRegistry().GetComponentType(data.mTypeIndex).addComponentFunction(*data.mECS, data.mEntityID, nullptr);

            };

        auto undoCommand = [](const AddComponentData& data)
            {
                data.mECS->RemoveComponent(data.mEntityID, data.mTypeIndex);
            };

        commandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Add Component (" + componentName + ")"));
    }

    void RemoveComponent(ECS& ecs, const EntityID entityID, const std::type_index& typeIndex, ECS& ecsBuffer, EditorCommandTracker& commandTracker)
    {
        const EntityID copyEntityID = ecs.CopyEntity(entityID, ecsBuffer);
        ecs.RemoveComponent(entityID, typeIndex);


        struct RemoveComponentData final
        {
            std::reference_wrapper<ECS> mECS;
            EntityID mEntityID;
            std::reference_wrapper<ECS> mECSBuffer;
            EntityID mCopyEntityID;
        };

        RemoveComponentData data
        {
            .mECS = ecs,
            .mEntityID = entityID,
            .mECSBuffer = ecsBuffer,
            .mCopyEntityID = copyEntityID
        };

        auto command = [](const RemoveComponentData& data)
            {
                SwapEntities(data.mECS.get(), data.mECSBuffer.get(), data.mEntityID, data.mCopyEntityID);
            };

        commandTracker.RegisterCommand(EditorCommand(data, command, command, "Remove Component"));
    }

    void ShowEntityAddButtons(ECS& ecs, EntityID& selectedEntityID, std::vector<EntityID>& rootEntities,
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

                SelectEntity(createdEntityID, selectedEntityID, commandTracker);

                commandTracker.EndComposite();
            }

            const std::string addCubeItem = "Add Cube" + imGuiTag;
            if (ImGui::MenuItem(addCubeItem.c_str()))
            {
                commandTracker.BeginComposite("Create Entity + Select Entity");

                const EntityID createdEntityID = CreateEntity(ecs, rootEntities, defaultParent, commandTracker);

                AddComponentToEntity(ecs, createdEntityID, std::type_index(typeid(MeshComponent)), "Mesh Component", commandTracker);

                SelectEntity(createdEntityID, selectedEntityID, commandTracker);

                commandTracker.EndComposite();
            }

            ImGui::EndPopup();
        }
    }

    void ReorderEntity(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, const std::size_t newIndex, EditorCommandTracker& commandTracker)
    {
        ASSERT(entityID != InvalidEntityID);

        struct ReorderEntityData final
        {
            ECS* mECS = nullptr;
            EntityID mEntityID;
            std::vector<EntityID>* mRootEntities = nullptr;
            std::size_t mNewIndex = std::numeric_limits<std::size_t>::max();
            std::size_t mOldIndex = std::numeric_limits<std::size_t>::max();
        };

        const EntityID parentID = GetParentEntity(ecs, entityID);

        const std::size_t oldIndex = GetEntityIndexInParent(ecs, entityID, rootEntities);

        if (oldIndex == newIndex)
        {
            return;
        }

        ReorderEntityData data
        {
            .mECS = &ecs,
            .mEntityID = entityID,
            .mRootEntities = &rootEntities,
            .mNewIndex = newIndex,
            .mOldIndex = oldIndex
        };

        auto doCommand = [](const ReorderEntityData& data)
            {
                const EntityID parentID = GetParentEntity(*data.mECS, data.mEntityID);

                if (parentID != InvalidEntityID)
                {
                    TransformHierarchyComponent* parentTransformComponent = data.mECS->GetComponent<TransformHierarchyComponent>(parentID);
                    std::iter_swap(begin(parentTransformComponent->children) + data.mOldIndex, begin(parentTransformComponent->children) + data.mNewIndex);
                }
                else
                {
                    std::iter_swap(begin(*data.mRootEntities) + data.mOldIndex, begin(*data.mRootEntities) + data.mNewIndex);
                }
            };

        auto undoCommand = doCommand;

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
            ECS* mECS = nullptr;
            EntityID mParentID;
            EntityID mChildID;
            EntityID mOldParentID;
            std::vector<EntityID>* mRootEntities = nullptr;
            std::size_t mChildIndex = 0;
            ChildIndexSetting mIndexSetting;
        };

        const EntityID oldParent = GetParentEntity(ecs, childID);
        SetParentEntityData data{};
        data.mECS = &ecs;
        data.mParentID = parentID;
        data.mChildID = childID;
        data.mOldParentID = oldParent;
        data.mRootEntities = &rootEntities;
        data.mIndexSetting = indexSetting;

        if (data.mOldParentID == data.mParentID)
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

            data.mChildIndex = std::ranges::distance(begin(rootEntities), it);
        }
        else
        {
            auto parentTransformComponent = ecs.GetComponent<TransformHierarchyComponent>(oldParent);
            auto it = std::ranges::find(parentTransformComponent->children, childID);
            if (it == end(parentTransformComponent->children))
            {
                throw std::runtime_error("Child entity not found in parent's children");
            }

            data.mChildIndex = std::ranges::distance(begin(parentTransformComponent->children), it);
        }

        auto doCommand = [](const SetParentEntityData& data)
            {
                SetParentEntity(*data.mECS, data.mChildID, data.mParentID, data.mIndexSetting);

                if (data.mOldParentID == InvalidEntityID)
                {
                    data.mRootEntities->erase(data.mRootEntities->begin() + data.mChildIndex);
                }
                else if (data.mParentID == InvalidEntityID)
                {
                    std::visit(Visitor
                        {
                            [&](const Index& index)
                            {
                                data.mRootEntities->insert(begin(*data.mRootEntities) + index.mIndex, data.mChildID);
                            },
                            [&](const FirstIndex&)
                            {
                                data.mRootEntities->insert(data.mRootEntities->begin(), data.mChildID);
                            },
                            [&](const LastIndex&)
                            {
                                data.mRootEntities->push_back(data.mChildID);
                            }
                        }, data.mIndexSetting);
                }
            };

        auto undoCommand = [](const SetParentEntityData& data)
            {
                SetParentEntity(*data.mECS, data.mChildID, data.mOldParentID, Index{ data.mChildIndex });

                if (data.mOldParentID == InvalidEntityID)
                {
                    data.mRootEntities->insert(data.mRootEntities->begin() + data.mChildIndex, data.mChildID);
                }
                else if (data.mParentID == InvalidEntityID)
                {
                    std::visit(Visitor
                        {
                            [&](const Index& index)
                            {
                                data.mRootEntities->erase(begin(*data.mRootEntities) + index.mIndex);
                            },
                            [&](const FirstIndex&)
                            {
                                data.mRootEntities->erase(data.mRootEntities->begin());
                            },
                            [&](const LastIndex&)
                            {
                                data.mRootEntities->pop_back();
                            }
                        }, data.mIndexSetting);
                }
            };

        commandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Add Child Entity"));
    }


    static void ShowEntityPayload(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
    {
        ObjectSource(entityID, "EntityID");

        if (const std::optional<EntityID> childEntityID = ObjectTarget<EntityID>())
        {
            SetParentEntity(ecs, entityID, childEntityID.value(), rootEntities, commandTracker);
        }
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

    static void ShowEntityOptionsPopUp(const std::string& entityOptionsPopUpName, ECS& ecs, EntityID selectedEntityID,
        std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker, const std::string& imGuiTag)
    {
        if (ImGui::BeginPopup(entityOptionsPopUpName.c_str()))
        {
            const std::string removeItem = "Remove" + imGuiTag;
            if (ImGui::MenuItem(removeItem.c_str()))
            {
                commandTracker.BeginComposite("Destroy Entity Composite");
                DestroyEntity(ecs, selectedEntityID, rootEntities, commandTracker);
                SelectEntity(InvalidEntityID, selectedEntityID, commandTracker);
                commandTracker.EndComposite();
            }

            std::string moveUpString = "Move Up" + imGuiTag;
            if (ImGui::MenuItem(moveUpString.c_str()))
            {
                commandTracker.BeginComposite("Move Entity Up");
                const EntityID parentID = GetParentEntity(ecs, selectedEntityID);
                const EntityID parentOfParentID = GetParentEntity(ecs, parentID);

                const std::size_t insertionIndex = GetEntityIndexInParent(ecs, parentID, rootEntities);


                SetParentEntity(ecs, parentOfParentID, selectedEntityID, rootEntities, commandTracker, Index{ insertionIndex });
                commandTracker.EndComposite();
            }

            ImGui::EndPopup();
        }
    }


    static void OnEntityDroppedBetween(ECS& ecs, const EntityID droppedEntityID, const EntityID droppedAfterEntityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
    {
        ASSERT(droppedEntityID != InvalidEntityID);
        ASSERT(droppedAfterEntityID != InvalidEntityID);
        if (droppedEntityID == droppedAfterEntityID)
        {
            return;
        }

        const std::size_t insertionIndex = GetEntityIndexInParent(ecs, droppedAfterEntityID, rootEntities);

        const EntityID parentID = GetParentEntity(ecs, droppedAfterEntityID);
        const EntityID currentParentID = GetParentEntity(ecs, droppedEntityID);

        if (parentID == currentParentID)
        {
            ReorderEntity(ecs, droppedEntityID, rootEntities, insertionIndex, commandTracker);
        }
        else
        {
            SetParentEntity(ecs, parentID, droppedEntityID, rootEntities, commandTracker, Index{ insertionIndex });
        }
    }

    static void ShowEntityChildren(ECS& ecs, const EntityID entityID, EntityID& selectedEntityID, ECS& ecsBuffer,
        std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker,
        const std::span<const EntityID> parentEntities, const std::string& imGuiTag, const std::set<EntityID>& uneditableEntities, const std::function<bool(EntityID)>& filter)
    {

        const bool isSelected = selectedEntityID == entityID;
        const NameComponent* nameComponent = ecs.GetComponent<NameComponent>(entityID);

        ASSERT(nameComponent != nullptr);
        const TransformHierarchyComponent* hierarchyComponent = ecs.GetComponent<TransformHierarchyComponent>(entityID);
        ASSERT(hierarchyComponent != nullptr);
        const bool isLeaf = hierarchyComponent->children.empty();
        const bool shouldBeDefaultOpen = std::ranges::find(parentEntities, entityID) != end(parentEntities);

        const bool isUneditable = uneditableEntities.contains(entityID);
        ImGui::PushID(entityID.id);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
        ImGui::InvisibleButton(std::to_string(entityID.id).c_str(), ImVec2{ ImGui::GetContentRegionAvail().x, 3 });
        ImGui::PopStyleVar();
        if (std::optional<EntityID> droppedEntityID = ObjectTarget<EntityID>())
        {
            OnEntityDroppedBetween(ecs, droppedEntityID.value(), entityID, rootEntities, commandTracker);
        }
        ImGui::PopID();
        ImGui::PushID(entityID.id);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 3 });
        const bool isOpen = ImGui::TreeNodeEx(nameComponent->name.c_str(), GetHierarchyTreeNodeFlags(isSelected, isLeaf, shouldBeDefaultOpen));
        ImGui::PopStyleVar();
        ImGui::PopID();

        const bool isLeftClicked = ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left);
        const bool isRightClicked = ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right);
        if (isRightClicked)
        {
            SelectEntity(entityID, selectedEntityID, commandTracker);
        }

        if (selectedEntityID == entityID)
        {
            ImGui::SetItemDefaultFocus();

            const std::string entityOptionsPopUpName = "Entity Settings" + imGuiTag;
            if (isRightClicked && !isUneditable)
            {
                ImGui::OpenPopup(entityOptionsPopUpName.c_str());
            }

            ShowEntityOptionsPopUp(entityOptionsPopUpName, ecs, selectedEntityID, rootEntities, commandTracker, imGuiTag);
        }

        ShowEntityPayload(ecs, entityID, rootEntities, commandTracker);

        if (isLeftClicked && !ImGui::IsItemToggledOpen())
        {
            SelectEntity(entityID, selectedEntityID, commandTracker);
        }

        if (isOpen)
        {
            for (const EntityID childEntityID : hierarchyComponent->children)
            {
                ASSERT(childEntityID != InvalidEntityID);
                if (!filter(childEntityID))
                {
                    continue;
                }
                ShowEntityChildren(ecs, childEntityID, selectedEntityID, ecsBuffer, rootEntities, commandTracker, parentEntities, imGuiTag, uneditableEntities, filter);
            }

            ImGui::TreePop();
        }
    }



   

    void ShowEntityHierarchy(ECS& ecs, ECS& ecsBuffer, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker,
        const std::string& imGuiTag, EntityID& selectedEntityID, const std::set<EntityID>& uneditableEntities, std::string& entitySearchBuffer)
    {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColor(0.18f, 0.18f, 0.18f, 0.80f).Value);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(0.12f, 0.12f, 0.12f, 0.0f).Value);
        ImGui::PushStyleColor(ImGuiCol_Border, ImColor(0.12f, 0.12f, 0.12f, 0.0f).Value);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

        auto filter = [&](EntityID entityID)
            {
                if (entitySearchBuffer.empty())
                {
                    return true;
                }
                auto nameSearch = [&](EntityID childID)
                    {
                        const NameComponent* nameComponent = ecs.GetComponent<NameComponent>(childID);
                        ASSERT(nameComponent != nullptr);
                        return CaseInsensitiveContains(nameComponent->name, entitySearchBuffer);
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

        if (ImGui::BeginListBox(listBoxName.c_str(), parentSize))
        {
            std::vector<EntityID> parentEntites;
            if (selectedEntityID != InvalidEntityID)
            {
                parentEntites = GetParents(ecs, selectedEntityID);
            }
            for (const EntityID rootEntityID : rootEntities)
            {
                if (!filter(rootEntityID))
                {
                    continue;
                }
                ShowEntityChildren(
                    ecs,
                    rootEntityID,
                    selectedEntityID,
                    ecsBuffer,
                    rootEntities,
                    commandTracker,
                    parentEntites,
                    imGuiTag,
                    uneditableEntities,
                    filter
                );
            }

            ImGui::EndListBox();
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }

    static void ShowEntitySearchBar(std::string& entitySearchString)
    {
        char searchBufferArray[256]{};
        if (ImGui::InputTextWithHint("##EntitySearch", "Search...", &searchBufferArray[0], sizeof(searchBufferArray)))
        {
            entitySearchString = searchBufferArray;
        }
        entitySearchString = searchBufferArray;
    }

    void ShowEntityHierarchyWithAddButtons(ECS& ecs, ECS& ecsBuffer, std::vector<EntityID>& rootEntities,
        EditorCommandTracker& commandTracker, const std::string& imGuiTag, EntityID& selectedEntityID, const EntityID defaultParent, const std::set<EntityID>& uneditableEntities, std::string& entitySearchBuffer)
    {
        ShowEntityAddButtons(ecs, selectedEntityID, rootEntities, commandTracker, imGuiTag, defaultParent);
        ImGui::Separator();
        ShowEntitySearchBar(entitySearchBuffer);
        ImGui::Separator();
        ShowEntityHierarchy(ecs, ecsBuffer, rootEntities, commandTracker, imGuiTag, selectedEntityID, uneditableEntities, entitySearchBuffer);
    }

    static void ShowComponentData(ECS& ecs, const EntityID entityID, const std::type_info& typeInfo, void* componentPtr, bool& anyActiveItem, ECS& ecsBuffer, const DataTypeRegistry& dataTypeRegistry, EditorCommandTracker& commandTracker, const Blackboard& blackboard)
    {
        PROFILER_FUNCTION(profiler::colors::Lime400);

        ASSERT(componentPtr != nullptr);
        ImGui::AlignTextToFramePadding();

        const DataTypeID componentDataTypeID = DataTypeID{ typeInfo.hash_code() };

        const DataType* dataType = dataTypeRegistry.Find(componentDataTypeID);
        if (dataType == nullptr)
        {
            return;
        }

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
            const ViewAndEditResult viewAndEditResult = dataTypeRegistry.ViewAndEditData(componentDataTypeID, componentPtr, blackboard);
            anyActiveItem |= viewAndEditResult.isActive;
            PROFILER_END();
        }

        if (ImGui::BeginPopup("Component Options"))
        {
            ImGui::BeginDisabled(ecs.GetRegistry().GetComponentType(typeInfo).isDefault);
            if (ImGui::MenuItem("Remove Component"))
            {
                RemoveComponent(ecs, entityID, typeInfo, ecsBuffer, commandTracker);
            }
            ImGui::EndDisabled();

            ImGui::EndPopup();
        }

        if (isOpen)
        {
            ImGui::TreePop();
        }

        ImGui::Separator();
    }

    void ShowEntityName(ECS& ecs, const EntityID selectedEntityID, const InputState& input, EditorCommandTracker& commandTracker)
    {
        if (selectedEntityID == InvalidEntityID)
        {
            return;
        }
        NameComponent* nameComponent = ecs.GetComponent<NameComponent>(selectedEntityID);
        if (nameComponent == nullptr)
        {
            throw std::runtime_error("Selected entity does not have a NameComponent");
        }
        std::string& selectedEntityName = nameComponent->name;

        char nameBuffer[256]{};
        CopyString(nameBuffer, selectedEntityName);

        ImGui::PushItemWidth(200);

        if (ImGui::InputTextWithHint("Name", "Entity Name", nameBuffer, sizeof(nameBuffer)))
        {
            if (input.IsKeyPressed(eInputKey::Enter))
            {
                struct SetNameData final
                {
                    ECS* mECS = nullptr;
                    EntityID mEntityID;
                    std::string mNewName;
                    std::string mOldName;
                };

                SetNameData data;
                data.mECS = &ecs;
                data.mEntityID = selectedEntityID;
                data.mNewName = nameBuffer;
                data.mOldName = selectedEntityName;

                auto doCommand = [](const SetNameData& data)
                    {
                        NameComponent* nameComponent = data.mECS->GetComponent<NameComponent>(data.mEntityID);
                        ASSERT(nameComponent != nullptr);
                        nameComponent->name = data.mNewName;
                    };

                auto undoCommand = [](const SetNameData& data)
                    {
                        NameComponent* nameComponent = data.mECS->GetComponent<NameComponent>(data.mEntityID);
                        ASSERT(nameComponent != nullptr);
                        nameComponent->name = data.mOldName;
                    };

                commandTracker.ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Set Entity Name"));
            }
        }

        ImGui::PopItemWidth();

        ImGui::SameLine(ImGui::GetWindowWidth() - 70);
        ImGui::Text(std::string("ID: " + std::to_string(selectedEntityID.id)).c_str());
        ImGui::Separator();

    }

    void ShowEntityComponents(ECS& ecs, const EntityID selectedEntityID, bool& anyItemActiveLastFrame,
        ECS& ecsBuffer, EntityID& copyEntityID, const Blackboard& blackboard, EditorCommandTracker& commandTracker)
    {
        PROFILER_FUNCTION(profiler::colors::Brown400);

        if (!anyItemActiveLastFrame)
        {
            copyEntityID = ecs.CopyEntity(selectedEntityID, ecsBuffer);
        }

        bool anyActiveItem = false;

        static bool showAllComponents = false;
        for (auto [typeInfo, componentPtr] : ecs.ViewEntity(selectedEntityID))
        {
            bool isHiddenComponent = (typeInfo == typeid(NameComponent) || typeInfo == typeid(TransformHierarchyComponent));
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
            ShowComponentData(ecs, selectedEntityID, typeInfo, componentPtr, anyActiveItem, ecsBuffer, blackboard.Get<Key_DataTypeRegistry>(), commandTracker, blackboard);
            if (isHiddenComponent && showAllComponents)
            {
                ImGui::EndDisabled();
            }
        }

        if (!anyActiveItem && anyItemActiveLastFrame)
        {

            struct ChangeComponentValueData final
            {
                ECS* mECS = nullptr;
                ECS* mECSBuffer = nullptr;
                EntityID mEntityID;
                EntityID mCopyEntityID;
            };

            ChangeComponentValueData data;
            data.mECS = &ecs;
            data.mECSBuffer = &ecsBuffer;
            data.mEntityID = selectedEntityID;
            data.mCopyEntityID = copyEntityID;

            auto command = [](const ChangeComponentValueData& data)
                {
                    SwapEntities(*data.mECS, *data.mECSBuffer, data.mEntityID, data.mCopyEntityID);
                };

            commandTracker.RegisterCommand(EditorCommand(data, command, command, "Change Component Value"));

            copyEntityID = InvalidEntityID;
        }

        if (!anyActiveItem && !anyItemActiveLastFrame)
        {
            ecsBuffer.DestroyEntity(copyEntityID);
        }

        anyItemActiveLastFrame = anyActiveItem;
    }

    void ShowEntityAddComponentButtons(ECS& ecs, const EntityID entityID, const DataTypeRegistry& dataTypeRegistry, EditorCommandTracker& commandTracker)
    {
        const std::string popupID = "Add Component##" + std::to_string(entityID.id);

        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup(popupID.c_str());
        }

        static uint32_t selectedIndex = 0;
        if (ImGui::BeginPopup(popupID.c_str()))
        {

            char searchBuffer[256]{};

            if (ImGui::InputTextWithHint("Search##Component", "Search", searchBuffer, IM_ARRAYSIZE(searchBuffer), ImGuiInputTextFlags_AutoSelectAll))
            {
                selectedIndex = 0;
            }

            const std::string searchString(searchBuffer);

            auto nameMatchesInput = [&searchString](const DataType& dataType)
                {
                    return searchString.empty() || CaseInsensitiveContains(dataType.prettyName, searchString);
                };

            auto isValidComponentDataType = [&ecs, entityID](const DataType& dataType)
                {
                    return dataType.isComponent
                        && !ecs.GetRegistry().GetComponentType(*dataType.typeInfo).isDefault
                        && !ecs.HasComponent(entityID, std::type_index(*dataType.typeInfo));
                };

            auto componentDataTypes = dataTypeRegistry.GetDataTypesFiltered(isValidComponentDataType)
                | std::views::values
                | std::views::filter(nameMatchesInput)
                | std::ranges::to<std::vector>();

            std::ranges::sort(componentDataTypes, [](const auto& a, const auto& b) { return a.prettyName < b.prettyName; });

            uint32_t indexCounter = 0;
            for (const auto& dataType : componentDataTypes)
            {
                const std::string componentNameLabel = dataType.prettyName + "##Component";
                if (ImGui::Selectable(componentNameLabel.c_str(), selectedIndex == indexCounter))
                {
                    AddComponentToEntity(ecs, entityID, std::type_index(*dataType.typeInfo), dataType.prettyName, commandTracker);
                    selectedIndex = 0;
                    ImGui::CloseCurrentPopup();
                }

                indexCounter++;
            }

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
                AddComponentToEntity(ecs, entityID, std::type_index(*dataType.typeInfo), dataType.prettyName, commandTracker);
                selectedIndex = 0;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        else
        {
            selectedIndex = 0;
        }
    }

    void ShowEntityInspector(ECS& ecs, const EntityID entityID, bool& anyItemActiveLastFrame,
        ECS& ecsBuffer, EntityID& copyEntityID, const Blackboard& blackboard, EditorCommandTracker& commandTracker)
    {
        PROFILER_FUNCTION(profiler::colors::Pink200);
        if (entityID == InvalidEntityID)
        {
            return;
        }
        ShowEntityComponents(ecs, entityID, anyItemActiveLastFrame, ecsBuffer, copyEntityID, blackboard, commandTracker);
        ShowEntityAddComponentButtons(ecs, entityID, blackboard.Get<Key_DataTypeRegistry>(), commandTracker);
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
        commandTracker;
        const EntityComposition& entityComposition = *compositionAsset.Get();
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