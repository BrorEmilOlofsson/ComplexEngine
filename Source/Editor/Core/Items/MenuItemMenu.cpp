#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/Core/Items/MenuItemMenu.hpp"

namespace Simple
{

	MenuItemMenu::MenuItemMenu(const std::string& name)
		: MenuItemBase(name)
	{
	}

	void MenuItemMenu::Render()
	{
		if (ImGui::BeginMenu(mName.c_str()))
		{
			for (const auto& child : mItems)
			{
				child->Render();
			}

			ImGui::EndMenu();
		}
	}
}
