#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/PopUps/Editor/SceneInspectorPopUp.hpp"
#include "Editor/Utility/EditorUtilities.hpp"
#include "Editor/Command/Core/EditorCommandTracker.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/AllEngineComponents.hpp"
#include "Editor/PopUps/Editor/SceneHierarchyPopUp.hpp"
#include "Editor/Functions/ECSEditorFunctions.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformUtility.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Engine/Scene/SceneManager.hpp"

namespace Simple
{

	static void ShowInspector(SceneManager& sceneManager, Camera& camera, const InputState& input, ECS& ecsBuffer, EditorCommandTracker& commandTracker,
		const Blackboard& blackboard, const std::string& imguiName, bool& isActive, const EntityID selectedEntityID, bool& anyItemActiveLastFrame, EntityID& copyEntityID)
	{
		if (ImGui::Begin(imguiName.c_str(), &isActive))
		{

			if (selectedEntityID == InvalidEntityID)
			{
				ImGui::End();
				return;
			}

			ECS& ecs = sceneManager.GetCurrentScene().GetECS();
			
			ShowEntityName(ecs, selectedEntityID, input);

			const Transform worldTransform = GetWorldTransform(ecs, selectedEntityID);

			Blackboard newBlackboard = blackboard;
			newBlackboard.Insert<Key_CurrentCamera>(camera);
			newBlackboard.Insert<Key_SceneRenderState>(sceneManager.GetCurrentScene().GetRenderState());
			newBlackboard.Insert<Key_ReferenceTransform>(worldTransform);
			ShowEntityInspector(ecs, selectedEntityID, anyItemActiveLastFrame, ecsBuffer, copyEntityID, newBlackboard, commandTracker);
		}

		ImGui::End();
	}

	SceneInspectorPopUp::SceneInspectorPopUp(const std::string& name, SceneHierarchyPopUp* sceneHierarchyPopUp, Camera* sceneCamera)
		: PopUp(name)
		, mHierarchyPopUp(sceneHierarchyPopUp)
		, mSceneCamera(sceneCamera)
	{
	}

	void SceneInspectorPopUp::Render(const Blackboard& blackboard)
	{
		PROFILER_FUNCTION(profiler::colors::Wheat);
		EditorCommandTracker& commandTracker = blackboard.Get<Key_CommandTracker>();
		SceneManager& sceneManager = blackboard.Get<Key_SceneManager>();
		ECS& ecsBuffer = blackboard.Get<Key_ECSBuffer>();
		const InputState& input = blackboard.Get<Key_InputState>();

		ShowInspector(
			sceneManager,
			*mSceneCamera,
			input,
			ecsBuffer,
			commandTracker,
			blackboard,
			mImGuiName,
			mIsActive,
			mHierarchyPopUp->GetSelectedEntityID(),
			mAnyItemActiveLastFrame,
			mCopyEntityID
		);
	}
}
