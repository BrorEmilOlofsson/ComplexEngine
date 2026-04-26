#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/PopUps/Editor/SceneHierarchyPopUp.hpp"
#include "Editor/Command/Core/EditorCommandTracker.hpp"
#include "Editor/Functions/ECSEditorFunctions.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Engine/ECSEngine/Utility/ECSUtilityFunctions.hpp"
#include "Editor/PopUps/Editor/EntityCompositionPopUp.hpp"

namespace CLX
{
    SceneHierarchyPopUp::SceneHierarchyPopUp(const std::string& aName)
        : PopUp(aName)
    {}

    void ShowSceneActive(SceneManager& sceneManager, const std::string& imGuiTag, const InputState& input)
    {
        const std::string sceneInputWithTextButton = "Scene" + imGuiTag;

        char sceneName[256]{};
        std::memset(sceneName, '\0', sizeof(sceneName));
        strncpy_s(sceneName, sceneManager.GetActiveScene().GetRelativePath().stem().string().c_str(), sizeof(sceneName));
        sceneName[sizeof(sceneName) - 1] = '\0';

        if (ImGui::InputTextWithHint(sceneInputWithTextButton.c_str(), "Name", sceneName, sizeof(sceneName)))
        {
            if (ImGui::IsItemFocused() && input.IsKeyPressed(eInputKey::Enter))
            {
                std::string newSceneName(sceneName);
                //sceneManager.ChangeSceneName(newSceneName);
            }
        }
    }

    static void CheckSelectedEntity(std::set<EntityID>& selectedEntityIDs, const ECS& ecs)
    {
        if (selectedEntityIDs.empty())
        {
            auto entityCollection = ecs.ViewEntities();
            if (!entityCollection.IsEmpty())
            {
                const auto entityView = *entityCollection.begin();
                selectedEntityIDs.insert(entityView.GetEntityID());
            }
        }
    }

    static void CheckCopyInputs(const InputState& input, ECS& ecs, std::set<EntityID>& selectedEntityIDs, ECS& ecsBuffer, EntityID& copiedEntityID, 
        std::vector<EntityID>& rootEntities, const DataTypeRegistry& dataTypeRegistry, EditorCommandTracker& commandTracker)
    {
        if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
        {
            return;
        }
        if (input.IsKeyPressed(eInputKey::Delete))
        {
            commandTracker.BeginComposite("Remove Entity + Select Entity");
            DestroyEntityHierarchies(ecs, selectedEntityIDs, rootEntities, commandTracker);
           
            ClearEntitySelection(selectedEntityIDs, commandTracker);
            commandTracker.EndComposite();
        }
        /*else if (input.IsKeyHeld(eInputKey::Ctrl) && input.IsKeyPressed(eInputKey::C))
        {
            copiedEntityID = ecs.CopyEntity(selectedEntityID, ecsBuffer);
        }*/
        else if (input.IsKeyHeld(eInputKey::Ctrl) && input.IsKeyPressed(eInputKey::V))
        {
            if (copiedEntityID != InvalidEntityID)
            {
                const EntityID createdEntityID = ecsBuffer.CopyEntity(copiedEntityID, ecs);

                SetEntitySelection(createdEntityID, selectedEntityIDs, commandTracker);
            }
        }
        else if (input.IsKeyHeld(eInputKey::Ctrl) && input.IsKeyPressed(eInputKey::D))
        {
            if (selectedEntityIDs.size() == 1)
            {
                const EntityID entityID = *selectedEntityIDs.begin();
                commandTracker.BeginComposite("Duplicate Entity + Select Entity");
                const Index index = Index{ GetEntityIndexInParent(ecs, entityID, rootEntities) + 1 };
                const EntityID newEntityID = DuplicateEntityHierarchy(ecs, entityID, rootEntities, index, dataTypeRegistry, commandTracker);
                SetEntitySelection(newEntityID, selectedEntityIDs, commandTracker);
                commandTracker.EndComposite();
            }
        }
    }

    void SceneHierarchyPopUp::UpdateInternal(const Blackboard& blackboard)
    {
        blackboard;
    }

    void SceneHierarchyPopUp::Render(const Blackboard& blackboard)
    {
        PROFILER_FUNCTION(profiler::colors::Olive);
        EditorCommandTracker& commandTracker = blackboard.Get<Key_CommandTracker>();
        SceneManager& sceneManager = blackboard.Get<Key_SceneManager>();
        DataTypeRegistry& dataTypeRegistry = blackboard.Get<Key_DataTypeRegistry>();
        ECS& ecs = sceneManager.GetActiveScene()->GetECS();
        ECS& ecsBuffer = blackboard.Get<Key_ECSBuffer>();
        const InputState& input = blackboard.Get<Key_InputState>();

        CheckSelectedEntity(mSelectedEntityIDs, ecs);

        if (ImGui::Begin(mImGuiName.c_str(), &IsActive()))
        {
            ShowEntityHierarchyWithAddButtons(
                ecs,
                ecsBuffer,
                mRootEntities,
                commandTracker,
                mImGuiTag,
                mSelectedEntityIDs,
                InvalidEntityID,
                {},
                mEntitySearchBuffer,
                dataTypeRegistry
            );


            EntityCompositionPopUp& popup = blackboard.Get<Key_EntityCompositionPopUp>();
            EntityCompositionAssetHandle openedEntityComposition = popup.GetCompositionAsset();
            ImGui::BeginDisabled(!openedEntityComposition.IsValid());
            if (ImGui::Button("Instantiate"))
            {
                InstantiateEntityComposition(
                    sceneManager.GetActiveScene()->GetECSHandle(), 
                    openedEntityComposition, 
                    InvalidEntityID,
                    dataTypeRegistry,
                    mRootEntities, 
                    commandTracker
                );
            }
            ImGui::EndDisabled();
        }

        CheckCopyInputs(
            input, 
            ecs, 
            mSelectedEntityIDs, 
            ecsBuffer, 
            mCopiedEntityID, 
            mRootEntities,
            dataTypeRegistry,
            commandTracker
        );

        ImGui::End();

    }

    void SceneHierarchyPopUp::OnSceneLoaded(Scene& scene)
    {
        mRootEntities = ::CLX::GetRootEntities(scene.GetECS());
        mSelectedEntityIDs.clear();
    }

    void SceneHierarchyPopUp::OnSceneBeginPlay(Scene&)
    {
        mStoredSelectedEntityIDs = mSelectedEntityIDs;
    }

    void SceneHierarchyPopUp::OnSceneEndPlay(Scene&)
    {
        mSelectedEntityIDs = mStoredSelectedEntityIDs;
        mStoredSelectedEntityIDs.clear();
    }

    std::set<EntityID>& SceneHierarchyPopUp::GetSelectedEntityIDs()
    {
        return mSelectedEntityIDs;
    }

    const std::set<EntityID>& SceneHierarchyPopUp::GetSelectedEntityIDs() const
    {
        return mSelectedEntityIDs;
    }

    std::vector<EntityID>& SceneHierarchyPopUp::GetRootEntities()
    {
        return mRootEntities;
    }

    const std::vector<EntityID>& SceneHierarchyPopUp::GetRootEntities() const
    {
        return mRootEntities;
    }
}