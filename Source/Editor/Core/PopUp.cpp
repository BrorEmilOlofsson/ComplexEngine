#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/Core/PopUp.hpp"

namespace Simple
{
	PopUp::PopUp(const std::string& windowName)
		: Window(windowName, "DefaultPopUp")
	{
		mIsActive = false;
	}

	void PopUp::Update(const Blackboard& blackboard)
	{
		if (IsActive())
		{
			if (ImGui::Begin(GetImGuiName().c_str(), &IsActive()))
			{
				mIsFocus = ImGui::IsWindowFocused();
			}

			ImGui::End();
		}

		UpdateInternal(blackboard);
	}
}