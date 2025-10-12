#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/Core/Items/MenuItemSelectable.hpp"

namespace Simple
{

	MenuItemSelectable::MenuItemSelectable(const std::string& name, std::function<void(const std::string&)> callback)
		: MenuItemBase(name)
		, mCallback(std::move(callback))
	{
	}

	void MenuItemSelectable::Render()
	{
		if (ImGui::BeginMenu(mName.c_str()))
		{
			for (const auto& name : mStrings)
			{
				if (ImGui::Selectable(name.c_str()) && mCallback)
				{
					mCallback(name);
				}
			}

			ImGui::EndMenu();
		}
	}

	void MenuItemSelectable::SetStrings(const std::vector<std::string>& aNewStrings)
	{
		mStrings = aNewStrings;
	}

	void MenuItemSelectable::SetCallback(std::function<void(const std::string&)> aCallback)
	{
		mCallback = std::move(aCallback);
	}
}
