#pragma once
#include "Utility/Shapes/AABB2.hpp"
#include "Engine/Camera/CameraMovement.hpp"
#include "Engine/OperatingSystem/WindowView.hpp"

namespace Simple
{

	class OperatingSystem;


	[[nodiscard]] AABB2i GetImGuiRenderRect();
	AABB2i RenderImage(void* textureID);

	void UpdateEditorCamera(Camera& camera, FreeFlyCameraSettings& settings, const float deltaTime, const WindowView windowView, const class InputState& input, OperatingSystem& os);

	void ShowSceneSettingsPopUp(struct EditorSceneSettings& editorSceneSettings);
}