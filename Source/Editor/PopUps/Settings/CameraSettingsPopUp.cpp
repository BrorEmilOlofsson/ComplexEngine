#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/PopUps/Settings/CameraSettingsPopUp.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Utility/Camera.hpp"
#include "Engine/Camera/CameraMovement.hpp"
#include "Engine/OperatingSystem/WindowView.hpp"
#include <External/imgui/imguizmo/imguizmo.h>

namespace Simple
{

	CameraSettingsPopUp::CameraSettingsPopUp(const std::string& name)
		: PopUp(name)
	{
	}

	void CameraSettingsPopUp::Render(const Blackboard& blackboard)
	{
		const WindowView windowView = blackboard.Get<Key_WindowView>();
		FreeFlyCameraSettings& cameraSettings = blackboard.Get<Key_FreeFlyCameraSettings>();
		if (ImGui::Begin(mImGuiName.c_str(), &mIsActive))
		{
			Camera& camera = blackboard.Get<Key_CurrentCamera>();
			Degreesf fov = ToDegrees(camera.GetHorizontalFOV());

			if (ImGui::DragFloat("Camera Move Speed", &cameraSettings.moveSpeed, 0.1f, 0.0f, 100.0f))
			{
			}

			if (ImGui::DragFloat("Camera Rotate Speed", &cameraSettings.rotationSpeed, 0.01f, 0.0f, 180.0f))
			{
			}

			if (ImGui::SliderFloat("FoV", &fov.Value(), 0.01f, 179.00f, "%0.2f"))
			{
				camera.SetHorizontalFOV(ToRadians(fov), windowView.GetClientSize());
			}

			Transform transform = camera.GetTransform();
			transform.SetPosition(Point3f(-5, 2, 0));
			Matrix4x4f matrix = transform.GetMatrix();

			auto view = camera.GetViewMatrix();

			ImGuizmo::Manipulate(&camera.GetViewMatrix()(1, 1), &camera.GetProjectionMatrix()(1, 1), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::LOCAL, &matrix(1, 1));

		}

		ImGui::End();
	}
}