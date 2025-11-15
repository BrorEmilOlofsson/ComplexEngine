#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorFunctions.hpp"
#include "Utility/ShapeMath.hpp"
#include "Graphics/RenderState.hpp"
#include "Utility/Math/TransformMode.hpp"
#include "Editor/EditorSceneSettings.hpp"

namespace Simple
{

	AABB2i GetImGuiRenderRect()
	{
		/*ImVec2 available = ImGui::GetContentRegionAvail();
		available -= ImVec2{ 5, 5 };*/

		//ImVec2 pos = ImGui::GetWindowPos();
		//ImVec2 size = ImGui::GetWindowSize();
		ImVec2 pos = ImGui::GetCursorScreenPos();    // Top-left of next item
		ImVec2 size =ImGui::GetContentRegionAvail();

		return AABB2i::FromMinAndExtent(
			ToPoint2<int>(pos),
			ToVector2<int>(size)
		);

	}

	AABB2i RenderImage(void* textureID)
	{
		ImVec2 available = ImGui::GetContentRegionAvail();
		//available -= ImVec2{ 5, 5 };

		// Ensure valid size (avoid 0x0)
		if (available.x < 1.0f || available.y < 1.0f)
		{
			return AABB2i();
		}

		ImGui::Image(textureID, available);

		ImVec2 imgMin = ImGui::GetItemRectMin();  // screen-space top-left of the image
		ImVec2 imgMax = ImGui::GetItemRectMax();  // screen-space bottom-right

		AABB2i rect = AABB2i::FromMinAndMax(
			{ (int)imgMin.x, (int)imgMin.y },
			{ (int)imgMax.x, (int)imgMax.y }
		);

		return rect;
	}

	void UpdateEditorCamera(Camera& camera, FreeFlyCameraSettings& settings, const float deltaTime, const WindowView windowView, const InputState& input, OSView os)
	{
		if (os.GetForegroundWindow() != windowView.GetHandle())
		{
			if (!os.IsCursorVisible())
			{
				os.ShowCursor();
				windowView.ReleaseCursor();
			}

			settings.freeFly = false;
			settings.mouseIsTapped = false;
			return;
		}

		if (input.IsKeyPressed(eInputKey::Tab) && !settings.mouseIsTapped)
		{
			settings.freeFly = !settings.freeFly;
		}

		const bool tapped = input.IsKeyHeld(eInputKey::RMB);

		if (tapped)
		{
			settings.mouseTapTimer += deltaTime;

			if (settings.mouseTapTimer > 0.12f)
			{
				settings.mouseTapTimer = 0.0f;
				settings.mouseIsTapped = true;
			}
		}
		else
		{
			settings.mouseTapTimer = 0.0f;
			settings.mouseIsTapped = false;
		}

		if (settings.freeFly || settings.mouseIsTapped)
		{
			if (os.IsCursorVisible())
			{
				os.HideCursor();
				windowView.CaptureCursor();
			}

			const int mouseWheelDelta = input.GetMouseWheelDelta();
			const float multiplier = mouseWheelDelta > 0 ? 0.15f : 0.15f;
			const float moveSpeedChange = mouseWheelDelta * settings.moveSpeed * multiplier;
			settings.moveSpeed = Max(settings.moveSpeed + moveSpeedChange, 0.f);

			float moveSpeed = settings.moveSpeed;
			if (input.IsKeyHeld(eInputKey::Shift))
			{
				moveSpeed *= 2.0f;
			}
			FreeFlyCameraUpdate(camera, deltaTime, moveSpeed, settings.rotationSpeed, input, settings.controls);

		}
		else
		{
			if (!os.IsCursorVisible())
			{
				os.ShowCursor();
				windowView.ReleaseCursor();
			}
		}
	}

	void ShowLocalWorldTransformSettings(eTransformMode& transformMode)
	{
		bool isLocal = transformMode == eTransformMode::Local;
		ImGui::Checkbox("Local", &isLocal);
		transformMode = isLocal ? eTransformMode::Local : eTransformMode::World;
	}

	void ShowGridSnapSettings(bool& useSnap, float& snapValue)
	{
		ImGui::Checkbox("Use Snap", &useSnap);
		ImGui::SetNextItemWidth(60.f);
		ImGui::DragFloat("Snap Value ", &snapValue, 1.f, 0.f, std::numeric_limits<float>::max() / std::numeric_limits<int>::max());
	}

	void ShowSceneSettingsPopUp(EditorSceneSettings& editorSceneSettings)
	{
		const ImVec2 previousCursorPos = ImGui::GetCursorPos();
		const ImVec2 sceneSettingsPopupPosition{ ImGui::GetWindowContentRegionMax().x - 70.f, ImGui::GetWindowContentRegionMin().y + 5.f };
		ImGui::SetCursorPos(sceneSettingsPopupPosition);

		if (ImGui::Button("Options", ImVec2{ 60.f, 25.f }))
		{
			ImGui::OpenPopup("Scene Settings##EditorSceneSettings");
		}

		ImGui::SetCursorPos(previousCursorPos);

		if (ImGui::BeginPopup("Scene Settings##EditorSceneSettings"))
		{
			ShowLocalWorldTransformSettings(editorSceneSettings.transformMode);
			ShowGridSnapSettings(editorSceneSettings.useSnap, editorSceneSettings.snapValue);
			ImGui::Checkbox("Show Grid", &editorSceneSettings.showGrid);
			ImGui::EndPopup();
		}
	}
}