#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/Core/Items/MenuItemButton.hpp"

namespace CLX
{
	MenuItemButton::MenuItemButton(const std::string& name, std::function<void()> callback)
		: MenuItemBase(name)
		, mCallback(std::move(callback))
	{
	}

	void MenuItemButton::Render()
	{
		if (ImGui::MenuItem(mName.c_str()) && mCallback)
		{
			mCallback();
		}
	}

	void MenuItemButton::SetCallback(std::function<void()> callback)
	{
		mCallback = callback;
	}
}
