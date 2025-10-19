#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/PopUps/Editor/SceneWindowPopUp.hpp"
#include "Engine/ECSEngine/Components/CameraComponent.hpp"
#include "Editor/Functions/ECSEditorFunctions.hpp"
#include "Editor/Editor.hpp"
#include "Editor/Functions/EditorFunctions.hpp"
#include "Utility/AssetPath.hpp"
#include "Engine/Navmesh/NavmeshLoader.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Engine/OperatingSystem/OSView.hpp"
#include "Editor/EditorSceneSettings.hpp"
#include "Engine/Utility/DebugShapes.hpp"
#include <External/AwsomeFontIcons/IconFontDefines.h>

namespace Simple
{

	static void ShowSceneData(SceneManager& sceneManager)
	{
		if (ImGui::Begin("Scene Data"))
		{
			Navmesh& navmesh = sceneManager.GetCurrentScene().GetNavmesh();

			ImGui::Text("Drag Navmesh Here");
			auto objAsset = ObjectTarget<AssetPath_OBJ>();
			if (objAsset)
			{
				const std::filesystem::path navmeshPath = std::filesystem::path(objAsset->Value().view());
				navmesh = Navmesh(NavmeshLoader::LoadMesh(navmeshPath));
				sceneManager.GetCurrentScene().SetNavmeshPath(navmeshPath);
			}

			SceneSettings& sceneSettings = sceneManager.GetSettings();
			NavmeshRenderSettings& navmeshRenderSettings = sceneSettings.navmeshRenderSettings;

			if (ImGui::Button("Toggle Navmesh Render"))
			{
				const bool newRenderState = !navmeshRenderSettings.renderLines;
				navmeshRenderSettings.renderLines = newRenderState;
				navmeshRenderSettings.renderNodeIndices = newRenderState;
				navmeshRenderSettings.renderVertexIndices = newRenderState;
				navmeshRenderSettings.renderNodeGrid = newRenderState;
			}
			ImGui::Checkbox("Render Navmesh Lines", &navmeshRenderSettings.renderLines);
			ImGui::Checkbox("Render Navmesh Node Indices", &navmeshRenderSettings.renderNodeIndices);
			ImGui::Checkbox("Render Navmesh Vertex Indices", &navmeshRenderSettings.renderVertexIndices);
			ImGui::Checkbox("Render Node Grid", &navmeshRenderSettings.renderNodeGrid);
		}
		ImGui::End();
	}

	static void RenderOrientationCube(const Camera& camera)
	{
		Matrix4x4f view = camera.GetViewMatrix();

		constexpr float cubeSize = 64.0f;

		const ImVec2 windowPos = ImGui::GetWindowPos();
		const ImVec2 windowSize = ImGui::GetWindowSize();

		const ImVec2 cubePos = ImVec2(
			windowPos.x + windowSize.x - cubeSize - 10.0f,
			windowPos.y + windowSize.y - cubeSize - 10.0f
		);

		ImGuizmo::ViewManipulate(view.GetDataPtr(), 16, cubePos, ImVec2(cubeSize, cubeSize), 0);
	}

	static void RenderPlayButton(Editor& editor, SceneManager& sceneManager)
	{
		ImGuiStyleManager& imguiStyleManager = editor.GetImGuiStyleManager();
		if (ImGui::BeginMainMenuBar())
		{
			const float distanceFromStart = ImGui::GetWindowWidth() - ImGui::GetContentRegionAvail().x;

			ImGui::Dummy(ImVec2(-distanceFromStart + ImGui::GetWindowWidth() * 0.5f - 38.0f, 0));

			const bool isPlaying = sceneManager.IsPlaying();
			const char* playIcon = isPlaying ? ICON_FA_PAUSE : ICON_FA_PLAY;

			if (isPlaying)
			{
				imguiStyleManager.SetEditorMode(eImGuiEditorMode::Playing);
			}
			else
			{
				imguiStyleManager.SetEditorMode(eImGuiEditorMode::Default);
			}

			if (isPlaying)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImColor(1.0f, 0.0f, 0.0f, 1.0f).Value);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(0.6f, 0.0f, 0.0f, 1.0f).Value);
			}

			if (ImGui::Button(playIcon))
			{
				const bool startPlaying = !isPlaying;

				if (startPlaying)
				{
					sceneManager.BeginPlay();
					editor.OnSceneBeginPlay(sceneManager.GetCurrentScene());
				}
				else
				{
					editor.OnSceneEndPlay(sceneManager.GetCurrentScene());
					sceneManager.EndPlay();
				}
			}

			if (isPlaying == true)
			{
				ImGui::PopStyleColor(2);
			}

			ImGui::EndMainMenuBar();
		}
	}


	SceneWindowPopUp::SceneWindowPopUp(const std::string& name)
		: PopUp(name)
		, mHierarchyPopUp("Hierarchy")
		, mInspectorPopUp("Inspector", &mHierarchyPopUp, &mCamera)
	{
		mCamera.SetRotation(Rotatorf(Degreesf(25), Degreesf(0), Degreesf(0)));
		mCamera.SetPosition(Point3f(1, 9, -12));
	}

	void SceneWindowPopUp::UpdateInternal(const Blackboard& blackboard)
	{
		if (blackboard.Get<Key_IsPlaying>())
		{
			return;
		}

		const WindowView windowView = blackboard.Get<Key_WindowView>();
		const InputState& input = blackboard.Get<Key_InputState>();
		const float deltaTime = blackboard.Get<Key_DeltaTime>();
		OSView os = blackboard.Get<Key_OSView>();
		EditorCommandTracker& commandTracker = blackboard.Get<Key_CommandTracker>();
		FreeFlyCameraSettings& cameraSettings = blackboard.Get<Key_FreeFlyCameraSettings>();
		if (IsFocused())
		{
			UpdateEditorCamera(mCamera, cameraSettings, deltaTime, windowView, input, os);
		}
		SceneManager& sceneManager = blackboard.Get<Key_SceneManager>();

		const EditorSceneSettings& editorSceneSettings = blackboard.Get<Key_EditorSceneSettings>();
		if (editorSceneSettings.showGrid)
		{
			const PrimitiveGrid3 grid
			{
				.minPos = Point3f::Zero(),
				.gridSize = Vector3ui(500, 0, 500),
				.cellSize = Vector3f(10, 0, 10),
				.offset = Vector3f(250.f, 0.f, 250.f),
			};
			RenderGrid3(grid, Colors::Gray, sceneManager.GetCurrentScene().GetRenderState().GetRenderList());
		}

		if (ImGui::Begin(mImGuiName.c_str()))
		{
			RenderState& sceneRenderState = sceneManager.GetCurrentScene().GetRenderState();
			AABB2i renderRect = GetImGuiRenderRect();
			sceneRenderState.SetRenderRect(renderRect);
			mCamera.SetResolution(Vector2ui(renderRect.GetExtent()));
			sceneRenderState.SetCamera(mCamera);

			sceneRenderState.mCursorScreenPos = os.GetCursorScreenPosition();

			if (input.IsKeyPressed(eInputKey::LMB))
			{
				const EntityID entityID{ sceneRenderState.mSelectedObjectID };
				if (entityID != InvalidEntityID)
				{
					SelectEntity(entityID, mHierarchyPopUp.GetSelectedEntityID(), commandTracker);
				}
			}

		}
		ImGui::End();
	}
	
	template<typename T>
	static float ToAspectRatio(Vector2<T> size)
	{
		return static_cast<float>(size.x) / static_cast<float>(size.y);
	}

	void SceneWindowPopUp::Render(const Blackboard& blackboard)
	{
		PROFILER_FUNCTION(profiler::colors::Yellow900);
		
		SceneManager& sceneManager = blackboard.Get<Key_SceneManager>();
		Blackboard newBlackboard = blackboard;
		newBlackboard.Insert<Key_CurrentRenderState>(sceneManager.GetCurrentScene().GetRenderState());
		EditorCommandTracker& commandTracker = blackboard.Get<Key_CommandTracker>();
		const InputState& input = blackboard.Get<Key_InputState>();
		const OSView os = blackboard.Get<Key_OSView>();
		EditorSceneSettings& editorSceneSettings = blackboard.Get<Key_EditorSceneSettings>();
		
		const WindowView windowView = blackboard.Get<Key_WindowView>();
		void* sceneTextureID = sceneManager.GetCurrentScene().GetRenderState().GetRenderTargetView()->GetSRV();
		RenderState& sceneRenderState = sceneManager.GetCurrentScene().GetRenderState();
		mHierarchyPopUp.Render(newBlackboard);
		mInspectorPopUp.Render(newBlackboard);

		if (input.IsKeyPressed(eInputKey::F))
		{
			TeleportCameraToEntity(sceneManager.GetCurrentScene().GetECS(), mHierarchyPopUp.GetSelectedEntityID(), mCamera, false);
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 3));

		if (ImGui::Begin(mImGuiName.c_str(), &mIsActive, ImGuiWindowFlags_NoScrollbar))
		{
			RenderOrientationCube(mCamera);

			const AABB2i renderRect = RenderImage(sceneTextureID);
			assert(renderRect == sceneRenderState.GetRenderRect().value());

			mTransformEntityTool.ShowEntityImGuizmo(
				sceneManager.GetCurrentScene().GetECS(),
				mHierarchyPopUp.GetSelectedEntityID(),
				editorSceneSettings.transformMode,
				sceneRenderState.GetRenderRect().value(),
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

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		if (ImGui::Begin(mImGuiName.c_str(), &mIsActive, ImGuiWindowFlags_NoScrollbar))
		{
			RenderPlayButton(blackboard.Get<Key_Editor>(), sceneManager);
		}

		ImGui::End();

		ShowSceneData(sceneManager);
	}

	void SceneWindowPopUp::OnSceneLoaded(Scene& scene)
	{
		mHierarchyPopUp.OnSceneLoaded(scene);
	}

	void SceneWindowPopUp::OnSceneBeginPlay(Scene& scene)
	{
		mHierarchyPopUp.OnSceneBeginPlay(scene);
	}

	void SceneWindowPopUp::OnSceneEndPlay(Scene& scene)
	{
		mHierarchyPopUp.OnSceneEndPlay(scene);
	}
}
