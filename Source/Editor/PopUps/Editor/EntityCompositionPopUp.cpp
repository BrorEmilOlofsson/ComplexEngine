#include "Editor/Precompiled/EditorPch.hpp"
#include "EntityCompositionPopUp.hpp"
#include "Editor/Functions/ECSEditorFunctions.hpp"
#include "Engine/ECSEngine/Utility/ECSEntityCompositionUtility.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Editor/Functions/EditorFunctions.hpp"
#include "Editor/EditorSceneSettings.hpp"
#include "Engine/Utility/DebugShapes.hpp"
#include "Engine/OperatingSystem/OperatingSystem.hpp"

namespace Simple
{

	EntityCompositionPopUp::EntityCompositionPopUp(const std::string&, RenderContext&& renderContext)
		: PopUp("Entity Composition Viewer")
		, mEntityComposition(ECSRegistry::Get())
	{
		mRenderState.SetRenderContext(std::move(renderContext));
	}

	void EntityCompositionPopUp::UpdateInternal(const Blackboard& blackboard)
	{
		Blackboard newBlackboard = blackboard;
		newBlackboard.Insert<Key_CurrentCamera>(mCamera);
		newBlackboard.Insert<Key_CurrentRenderState>(mRenderState);

		mEntityComposition.GetECS().EditorUpdate(newBlackboard);

		mRenderState.GetRenderList().AddSphere(DrawSphere{ Spheref(Point3f(0, 0, 10), 5.f), Colors::Salmon });

		if (IsFocused())
		{
			FreeFlyCameraSettings& cameraSettings = blackboard.Get<Key_FreeFlyCameraSettings>();
			const float deltaTime = blackboard.Get<Key_DeltaTime>();
			WindowView window = blackboard.Get<Key_WindowView>();
			const InputState& input = blackboard.Get<Key_InputState>();
			const OSView os = blackboard.Get<Key_OSView>();
			UpdateEditorCamera(mCamera, cameraSettings, deltaTime, window, input, os);
		}

		if (blackboard.Get<Key_EditorSceneSettings>().showGrid)
		{
			const PrimitiveGrid3 grid
			{
				.minPos = Point3f::Zero(),
				.gridSize = Vector3ui(50, 0, 50),
				.cellSize = Vector3f(10, 0, 10),
				.offset = Vector3f(250.f, 0.f, 250.f),
			};
			RenderGrid3(grid, Colors::Gray, mRenderState.GetRenderList());
		}

		const AABB2i renderRect = GetImGuiRenderRect();
		mRenderState.SetRenderRect(renderRect);
		mCamera.SetResolution(Vector2ui(renderRect.GetExtent()));
		mRenderState.SetCamera(mCamera);
	}

	void EntityCompositionPopUp::Render(const Blackboard& blackboard)
	{
		ECS& ecsBuffer = blackboard.Get<Key_ECSBuffer>();
		EditorCommandTracker& commandTracker = blackboard.Get<Key_CommandTracker>();
		const WindowView windowView = blackboard.Get<Key_WindowView>();
		SceneManager& sceneManager = blackboard.Get<Key_SceneManager>();
		OSView os = blackboard.Get<Key_OSView>();
		EditorSceneSettings& editorSceneSettings = blackboard.Get<Key_EditorSceneSettings>();
		const InputState& input = blackboard.Get<Key_InputState>();
		input;
		Blackboard newBlackboard = blackboard;
		newBlackboard.Insert<Key_CurrentCamera>(mCamera);
		newBlackboard.Insert<Key_CurrentRenderState>(mRenderState);
		mEntityComposition.GetECS().Render(newBlackboard);
		blackboard.Get<Key_OperatingSystem>().Render(mRenderState);

		if (ImGui::Begin("Entity Composition Hierarchy"))
		{
			if (ImGui::Button("Instantiate"))
			{
				MergeEntityComposition(mEntityComposition, sceneManager.GetCurrentScene().GetECS());
			}
			ShowEntityHierarchyWithAddButtons(mEntityComposition.GetECS(), ecsBuffer, mRootEntities, commandTracker, mImGuiTag, mSelectedEntityID);
		}

		ImGui::End();

		if (ImGui::Begin("Entity Composition Inspector"))
		{
			ShowEntityInspector(
				mEntityComposition.GetECS(), 
				mSelectedEntityID, 
				mAnyItemActiveLastFrame, 
				ecsBuffer,
				mCopyEntityID, 
				blackboard, 
				commandTracker
			);
		}
		ImGui::End();

		if (ImGui::Begin(GetImGuiName().c_str()))
		{
			const AABB2i renderRect = RenderImage(mRenderState.GetRenderContext()->GetOutputSRV());

			//mRenderState.SetRenderRect(renderRect);*/

			/*mTransformEntityTool.ShowEntityImGuizmo(
				mEntityComposition.GetECS(),
				mSelectedEntityID, 
				editorSceneSettings.transformMode,
				renderRect,
				editorSceneSettings.useSnap,
				editorSceneSettings.snapValue,
				mCamera,
				input, 
				os, 
				commandTracker
			);*/

			ShowSceneSettingsPopUp(editorSceneSettings);
		}

		ImGui::End();


		mRenderState.Reset();
	}

}