#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/Core/PopUp.hpp"

namespace CLX
{
	PopUp::PopUp(const std::string& windowName)
		: EditorWindow(windowName, "DefaultPopUp")
	{
		mIsActive = false;
	}

	void PopUp::Update(const Blackboard& blackboard)
	{
		bool& isActive = IsActive();
		if (isActive)
		{
			ImGui::Begin(GetImGuiName().c_str(), &IsActive());

			mIsFocus = ImGui::IsWindowFocused();

			ImGui::End();
		}
		else
		{
			IsActive() = false;
		}

		UpdateInternal(blackboard);
	}
}