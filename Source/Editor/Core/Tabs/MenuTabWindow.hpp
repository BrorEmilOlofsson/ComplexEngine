#pragma once
#include "Editor/Core/Base/MenuItemTabBase.hpp"
#include "Editor/Core/Items/MenuItemPopUp.hpp"
#include <vector>
#include <memory>

namespace CLX
{
	class MenuTabWindow final : public MainMenuTabBase
	{
	public:
		MenuTabWindow(const std::string& name, const std::string& imGuiTag);

		void Render() override final;

		void ActivateWindow(MenuItemPopUp* itemPopUp, const bool shouldActive = true);

		MenuItemPopUp* AddPopUp(const std::string& popUpButtonName);

	private:

		std::vector<std::unique_ptr<MenuItemPopUp>> mButtons;
	};
}