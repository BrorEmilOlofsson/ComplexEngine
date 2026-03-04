#include "Editor/Precompiled/EditorPch.hpp"
#include "CameraSettingsWindow.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Engine/Utility/Camera.hpp"
#include "Engine/Camera/CameraMovement.hpp"

namespace Simple
{

    static void ShowCameraSettings(FreeFlyCameraSettings& cameraSettings)
	{
		if (ImGui::DragFloat("Camera Move Speed", &cameraSettings.moveSpeed, 0.1f, 0.0f, 100.0f))
		{
		}
		if (ImGui::DragFloat("Camera Rotate Speed", &cameraSettings.rotationSpeed, 0.01f, 0.0f, 180.0f))
		{
		}
	}

	void ShowCameraSettingsWindow(FreeFlyCameraSettings& cameraSettings, bool& isActive)
	{
		if (!isActive)
		{
			return;
		}
		if (ImGui::Begin("Camera Settings", &isActive))
		{

			ShowCameraSettings(cameraSettings);
		}

		ImGui::End();
    }
}