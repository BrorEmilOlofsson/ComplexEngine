#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/PopUps/Editor/SceneInspectorPopUp.hpp"
#include "Editor/Command/Core/EditorCommandTracker.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Editor/PopUps/Editor/SceneHierarchyPopUp.hpp"
#include "Editor/Functions/ECSEditorFunctions.hpp"
#include "Engine/ECSEngine/Utility/ECSUtilityFunctions.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Engine/Scene/SceneManager.hpp"

namespace CLX
{

    static void ShowInspector(SceneManager& sceneManager, Camera& camera, const InputState& input, ECS& ecsBuffer, EditorCommandTracker& commandTracker,
        const Blackboard& blackboard, const std::string& imguiName, bool& isActive, const std::set<EntityID>& selectedEntityIDs, bool& anyItemActiveLastFrame, EntityID& copyEntityID,
        uint32_t& selectedComponentPopupIndex, std::string& componentSearchString, JsonAny& copiedComponent, EntityCompositionInstantiationManager& entityCompositionInstantiations,
        std::function<void(EntityID)>& onEntitySelected)
    {
        if (ImGui::Begin(imguiName.c_str(), &isActive))
        {
            const EntityID selectedEntityID = selectedEntityIDs.size() == 1 ? *selectedEntityIDs.begin() : InvalidEntityID;
            if (selectedEntityID == InvalidEntityID)
            {
                ImGui::End();
                return;
            }

            ECS& ecs = sceneManager.GetActiveScene()->GetECS();

            auto entityNameAction = ShowEntityName(ecs, selectedEntityID, input, true);

            const Transform worldTransform = GetEntityWorldTransform(ecs, selectedEntityID);

            Blackboard newBlackboard = blackboard;
            newBlackboard.Insert<Key_CurrentCamera>(camera);
            newBlackboard.Insert<Key_SceneRenderState>(sceneManager.GetActiveScene()->GetRenderState());
            newBlackboard.Insert<Key_ReferenceTransform>(worldTransform);
            newBlackboard.Insert<Key_OnEntitySelected>(onEntitySelected);
            newBlackboard.Insert<Key_CurrentECS>(ecs);
            auto editorActions = ShowEntityInspector(ShowEntityInspectorData
                {
                    ecs,
                    selectedEntityID,
                    anyItemActiveLastFrame,
                    ecsBuffer,
                    copyEntityID,
                    selectedComponentPopupIndex,
                    componentSearchString,
                    copiedComponent,
                    EntityCompositionAssetHandle::Empty(),
                    entityCompositionInstantiations,
                    newBlackboard
                }
            );

            if (entityNameAction)
            {
                entityNameAction.value()(commandTracker);
            }
            for (auto& action : editorActions)
            {
                action(commandTracker);
            }
        }

        ImGui::End();
    }

    SceneInspectorPopUp::SceneInspectorPopUp(const std::string& name, SceneHierarchyPopUp* sceneHierarchyPopUp, Camera* sceneCamera)
        : PopUp(name)
        , mHierarchyPopUp(sceneHierarchyPopUp)
        , mSceneCamera(sceneCamera)
    {}

    void SceneInspectorPopUp::Render(const Blackboard& blackboard)
    {
        PROFILER_FUNCTION(profiler::colors::Wheat);
        EditorCommandTracker& commandTracker = blackboard.Get<Key_CommandTracker>();
        SceneManager& sceneManager = blackboard.Get<Key_SceneManager>();
        ECS& ecsBuffer = blackboard.Get<Key_ECSBuffer>();
        const InputState& input = blackboard.Get<Key_InputState>();
        EntityCompositionInstantiationManager& entityCompositionInstantiations = blackboard.Get<Key_EntityCompositionInstantiationManager>();

        ShowInspector(
            sceneManager,
            *mSceneCamera,
            input,
            ecsBuffer,
            commandTracker,
            blackboard,
            mImGuiName,
            mIsActive,
            mHierarchyPopUp->GetSelectedEntityIDs(),
            mAnyItemActiveLastFrame,
            mCopyEntityID,
            mSelectedComponentPopupIndex,
            mComponentSearchBuffer,
            mCopiedComponent,
            entityCompositionInstantiations,
            mEntitySelectedCallback
        );
    }
}
