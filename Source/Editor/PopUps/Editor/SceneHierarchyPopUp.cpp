#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/PopUps/Editor/SceneHierarchyPopUp.hpp"
#include "Editor/Command/Core/EditorCommandTracker.hpp"
#include "Editor/Functions/ECSEditorFunctions.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformHierarchyUtility.hpp"
#include "Editor/PopUps/Editor/EntityCompositionPopUp.hpp"

namespace CLX
{
	SceneHierarchyPopUp::SceneHierarchyPopUp(const std::string& aName)
		: PopUp(aName)
	{
	}

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

	static void CheckSelectedEntity(EntityID& selectedEntityID, const ECS& ecs)
	{
		if (selectedEntityID == InvalidEntityID)
		{
			auto entityCollection = ecs.ViewEntities();
			if (!entityCollection.IsEmpty())
			{
				const auto entityView = *entityCollection.begin();
				selectedEntityID = entityView.GetEntityID();
			}
		}
	}

	static void CheckCopyInputs(const InputState& input, ECS& ecs, EntityID& selectedEntityID, ECS& ecsBuffer, EntityID& copiedEntityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker)
	{
		if (input.IsKeyPressed(eInputKey::Delete))
		{
			commandTracker.BeginComposite("Remove Entity + Select Entity");
			DestroyEntity(ecs, selectedEntityID, rootEntities, commandTracker);
			auto entityCollection = ecs.ViewEntities();

			const EntityID newSelectedEntityID = entityCollection.IsEmpty() ? InvalidEntityID : EntityID{ selectedEntityID.id == 0 ? selectedEntityID.id + 1 : selectedEntityID.id - 1 };

			SelectEntity(newSelectedEntityID, selectedEntityID, commandTracker);
			commandTracker.EndComposite();
		}
		else if (input.IsKeyHeld(eInputKey::Ctrl) && input.IsKeyPressed(eInputKey::C))
		{
			copiedEntityID = ecs.CopyEntity(selectedEntityID, ecsBuffer);
		}
		else if (input.IsKeyHeld(eInputKey::Ctrl) && input.IsKeyPressed(eInputKey::V))
		{
			if (copiedEntityID != InvalidEntityID)
			{
				const EntityID createdEntityID = ecsBuffer.CopyEntity(copiedEntityID, ecs);

				SelectEntity(createdEntityID, selectedEntityID, commandTracker);
			}
		}
		else if (input.IsKeyHeld(eInputKey::Ctrl) && input.IsKeyPressed(eInputKey::D))
		{

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
		ECS& ecs = sceneManager.GetActiveScene()->GetECS();
		ECS& ecsBuffer = blackboard.Get<Key_ECSBuffer>();
		const InputState& input = blackboard.Get<Key_InputState>();

		CheckSelectedEntity(mSelectedEntityID, ecs);

		if (ImGui::Begin(mImGuiName.c_str(), &IsActive()))
		{
			ShowEntityHierarchyWithAddButtons(
				ecs, 
				ecsBuffer, 
				mRootEntities, 
				commandTracker, 
				mImGuiTag, 
				mSelectedEntityID, 
				InvalidEntityID,
				{}
			);

			if (ImGui::Button("Instantiate"))
			{
				EntityCompositionPopUp& popup = blackboard.Get<Key_EntityCompositionPopUp>();
				InstantiateEntityComposition(sceneManager.GetActiveScene()->GetECS(), popup.GetCompositionAsset(), mRootEntities, commandTracker);
			}
		}

		ImGui::End();

		CheckCopyInputs(input, ecs, mSelectedEntityID, ecsBuffer, mCopiedEntityID, mRootEntities, commandTracker);
	}

	void SceneHierarchyPopUp::OnSceneLoaded(Scene& scene)
	{
		mRootEntities = ::CLX::GetRootEntities(scene.GetECS());
		mSelectedEntityID = InvalidEntityID;
	}

	void SceneHierarchyPopUp::OnSceneBeginPlay(Scene&)
	{
		mStoredSelectedEntityID = mSelectedEntityID;
	}

	void SceneHierarchyPopUp::OnSceneEndPlay(Scene&)
	{
		mSelectedEntityID = mStoredSelectedEntityID;
		mStoredSelectedEntityID = InvalidEntityID;
	}

	EntityID& SceneHierarchyPopUp::GetSelectedEntityID()
	{
		return mSelectedEntityID;
	}

	EntityID SceneHierarchyPopUp::GetSelectedEntityID() const
	{
		return mSelectedEntityID;
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