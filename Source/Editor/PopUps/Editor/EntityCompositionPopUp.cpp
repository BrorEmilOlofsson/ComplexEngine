#include "Editor/Precompiled/EditorPch.hpp"
#include "EntityCompositionPopUp.hpp"
#include "Editor/Functions/ECSEditorFunctions.hpp"
#include "Engine/ECS/EntityComposition.hpp"
#include "Engine/ECSEngine/Utility/ECSEntityCompositionUtility.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Editor/Functions/EditorFunctions.hpp"
#include "Editor/EditorSceneSettings.hpp"
#include "Engine/Utility/DebugShapes.hpp"
#include "Engine/OperatingSystem/OperatingSystem.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformUtility.hpp"
#include "Engine/ECSEngine/Components/EntityCompositionComponent.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformHierarchyUtility.hpp"
#include "Engine/ECS/ECSSerializer.hpp"

namespace Simple
{

	EntityCompositionPopUp::EntityCompositionPopUp(const std::string&, RenderContext&& renderContext)
		: PopUp("Entity Composition Viewer")
	{
		mRenderState.SetRenderContext(std::move(renderContext));
	}

	void EntityCompositionPopUp::UpdateInternal(const Blackboard& blackboard)
	{
		if (!mEntityCompositionAsset.IsValid())
		{
			std::shared_ptr<EntityComposition> ec = std::make_shared<EntityComposition>(ECSRegistry::Get());
			const std::filesystem::path defaultPath = std::filesystem::absolute(SIMPLE_DIR_ASSETS) / "EntityCompositions" / "Test.json";
			blackboard.Get<Key_AssetManager>().AddEntityCompositionAsset(defaultPath, EntityCompositionAsset(ec));

			mEntityCompositionAsset = blackboard.Get<Key_AssetManager>().GetEntityComposition(defaultPath);
			mRootEntities = GetRootEntities(mEntityCompositionAsset->GetECS());
			mEntityCompositionAsset->GetECS().GetComponent<NameComponent>(mEntityCompositionAsset->GetRootEntity())->name = "Root";
		}

		if (!IsActive())
		{
			return;
		}

		Blackboard newBlackboard = blackboard;
		newBlackboard.Insert<Key_CurrentCamera>(mCamera);
		newBlackboard.Insert<Key_CurrentRenderState>(mRenderState);
		const InputState& input = blackboard.Get<Key_InputState>();
		EditorCommandTracker& commandTracker = blackboard.Get<Key_CommandTracker>();
		const OSView os = blackboard.Get<Key_OSView>();
		mEntityCompositionAsset->GetECS().EditorUpdate(newBlackboard);

		constexpr Spheref drawSphere = Spheref::FromCenterAndRadius(Point3f(0, 0, 10), 5.f);
		mRenderState.GetRenderList().AddSphere(DrawSphere{ drawSphere, Colors::Salmon }, true);

		if (IsFocused())
		{
			FreeFlyCameraSettings& cameraSettings = blackboard.Get<Key_FreeFlyCameraSettings>();
			const float deltaTime = blackboard.Get<Key_DeltaTime>();
			WindowView window = blackboard.Get<Key_WindowView>();
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

		if (ImGui::Begin(mImGuiName.c_str()))
		{
			const AABB2i renderRect = GetImGuiRenderRect();
			mRenderState.SetRenderRect(renderRect);
			mCamera.SetResolution(Vector2ui(renderRect.GetExtent()));


			if (input.IsKeyPressed(eInputKey::LMB))
			{
				const Point2i mouseScreenPosition = os.GetCursorScreenPosition();

				if (IsInsideRenderRect(mouseScreenPosition, renderRect))
				{
					const Point2i mappedPos = MapToRenderRect(mouseScreenPosition, renderRect);

					const uint32_t id = mRenderState.GetRenderContext()->GetObjectIDAt(mappedPos);

					const EntityID entityID{ id };
					if (entityID != InvalidEntityID)
					{
						SelectEntity(entityID, mSelectedEntityID, commandTracker);
					}
				}
			}

		}
		ImGui::End();
		mRenderState.SetCamera(mCamera);
	}

	void EntityCompositionPopUp::Render(const Blackboard& blackboard)
	{
		ECS& ecsBuffer = blackboard.Get<Key_ECSBuffer>();
		EditorCommandTracker& commandTracker = blackboard.Get<Key_CommandTracker>();
		const WindowView windowView = blackboard.Get<Key_WindowView>();
		//SceneManager& sceneManager = blackboard.Get<Key_SceneManager>();
		OSView os = blackboard.Get<Key_OSView>();
		EditorSceneSettings& editorSceneSettings = blackboard.Get<Key_EditorSceneSettings>();
		const InputState& input = blackboard.Get<Key_InputState>();
		input;
		Blackboard newBlackboard = blackboard;
		newBlackboard.Insert<Key_CurrentCamera>(mCamera);
		newBlackboard.Insert<Key_CurrentRenderState>(mRenderState);
		mEntityCompositionAsset->GetECS().Render(newBlackboard);
		blackboard.Get<Key_OperatingSystem>().Render(mRenderState);

		if (ImGui::Begin("Entity Composition Hierarchy"))
		{
			if (ImGui::Button("Save"))
			{
				mEntityCompositionAsset->SetPath(std::filesystem::path(SIMPLE_DIR_ASSETS) / "EntityCompositions/Test.json");
				SaveEntityCompositionAsset(mEntityCompositionAsset, blackboard.Get<Key_DataTypeRegistry>());
			}
			ShowEntityHierarchyWithAddButtons(
				mEntityCompositionAsset->GetECS(),
				ecsBuffer,
				mRootEntities,
				commandTracker,
				mImGuiTag,
				mSelectedEntityID,
				mEntityCompositionAsset->GetRootEntity(),
				{ mEntityCompositionAsset->GetRootEntity() }
			);
		}

		ImGui::End();

		if (ImGui::Begin("Entity Composition Inspector"))
		{
			if (mSelectedEntityID != InvalidEntityID)
			{
				const Transform transform = GetWorldTransform(mEntityCompositionAsset->GetECS(), mSelectedEntityID);
				newBlackboard.Insert<Key_ReferenceTransform>(transform);
			}

			ShowEntityName(mEntityCompositionAsset->GetECS(), mSelectedEntityID, input);

			ShowEntityInspector(
				mEntityCompositionAsset->GetECS(),
				mSelectedEntityID,
				mAnyItemActiveLastFrame,
				ecsBuffer,
				mCopyEntityID,
				newBlackboard,
				commandTracker
			);
		}
		ImGui::End();

		if (ImGui::Begin(GetImGuiName().c_str()))
		{
			const AABB2i renderRect = RenderImage(mRenderState.GetRenderContext()->GetOutputSRV());

			//mRenderState.SetRenderRect(renderRect);*/

			mTransformEntityTool.ShowEntityImGuizmo(
				mEntityCompositionAsset->GetECS(),
				mSelectedEntityID,
				editorSceneSettings.transformMode,
				renderRect,
				editorSceneSettings.useSnap,
				editorSceneSettings.snapValue,
				mCamera,
				input,
				os,
				commandTracker
			);

			ShowSceneSettingsPopUp(editorSceneSettings);
		}

		ImGui::End();


		mRenderState.Reset();
	}


	void EntityCompositionPopUp::SetCompositionAsset(EntityCompositionAssetHandle asset)
	{
		if (!asset)
		{
			return;
		}
		if (asset == mEntityCompositionAsset)
		{
			return;
		}
		mEntityCompositionAsset = asset;
		mRootEntities = GetRootEntities(mEntityCompositionAsset->GetECS());
		mSelectedEntityID = mRootEntities.front();
	}

}