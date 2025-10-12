#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/PopUps/Help/CameraControlsGuidePopUp.hpp"

namespace Simple
{

	CameraControlsGuidePopUp::CameraControlsGuidePopUp(const std::string& name)
		: PopUp(name)
	{
	}

	void CameraControlsGuidePopUp::Render(const Blackboard&)
	{
		if (ImGui::Begin(mImGuiName.c_str(), &mIsActive))
		{
			ImGui::Text("LookAround            - Hold Right-Click");
			ImGui::Text("Toggle FreeFly        - TAB");

			ImGuiUtility::SeparatorTextDummy("In Freefly Mode");

			ImGui::Text("Movements             - W/A/S/D");
			ImGui::Text("Rotate Right          - E");
			ImGui::Text("Rotate Left           - Q");
			ImGui::Text("Rotate Up             - C");
			ImGui::Text("Rotate Down           - Z");
			ImGui::Text("Fly Up                - SPACE");
			ImGui::Text("Fly Down              - Hold Ctrl + SPACE");
			ImGui::Text("Speed x5              - Hold Shift");
			ImGui::Text("Increase Movespeed    - Scroll Forward/Backward");
		}

		ImGui::End();
	}
}
