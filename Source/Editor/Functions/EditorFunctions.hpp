#pragma once
#include "Utility/Shapes/AABB2.hpp"
#include "Engine/Camera/CameraMovement.hpp"
#include "Engine/OperatingSystem/WindowView.hpp"
#include "Engine/OperatingSystem/OSView.hpp"

namespace Simple
{
	[[nodiscard]] AABB2i GetImGuiRenderRect();
	AABB2i RenderImage(void* textureID);

	void UpdateEditorCamera(Camera& camera, FreeFlyCameraSettings& settings, const float deltaTime, const WindowView windowView, const class InputState& input, OSView os);

	void ShowSceneSettingsPopUp(struct EditorSceneSettings& editorSceneSettings);
}