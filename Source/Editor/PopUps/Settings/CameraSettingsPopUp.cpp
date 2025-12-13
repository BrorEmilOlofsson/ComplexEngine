#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/PopUps/Settings/CameraSettingsPopUp.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Utility/Camera.hpp"
#include "Engine/Camera/CameraMovement.hpp"

namespace Simple
{

	void ShowCameraSettings(FreeFlyCameraSettings& cameraSettings, bool& isActive)
	{
		if (!isActive)
		{
			return;
		}
		if (ImGui::Begin("Camera Settings", &isActive))
		{

			if (ImGui::DragFloat("Camera Move Speed", &cameraSettings.moveSpeed, 0.1f, 0.0f, 100.0f))
			{
			}

			if (ImGui::DragFloat("Camera Rotate Speed", &cameraSettings.rotationSpeed, 0.01f, 0.0f, 180.0f))
			{
			}
		}

		ImGui::End();
    }
}