#pragma once
#include "Editor/Core/Base/MenuItemTabBase.hpp"
#include "Editor/Core/Items/MenuItemButton.hpp"
#include "Editor/Core/Items/MenuItemPopUp.hpp"
#include "Editor/Core/Items/MenuItemMenu.hpp"
#include "Editor/Core/Items/MenuItemSelectable.hpp"
#include <memory>
#include <functional>
#include <vector>

namespace CLX
{

	class MenuTabDefault final : public MainMenuTabBase
	{
	public:

		MenuTabDefault(const std::string& name, const std::string& imGuiTag);

		void Render() override final;

	public:

		MenuItemButton* AddButton(const std::string& buttonName);
		MenuItemPopUp* AddPopUp(const std::string& popUpName);
		MenuItemMenu* AddMenu(const std::string& menuButtonName);
		MenuItemSelectable* AddSelectable(const std::string& selectableButtonName);

	private:
		std::vector<std::unique_ptr<MenuItemBase>> mButtons;
	};
}
