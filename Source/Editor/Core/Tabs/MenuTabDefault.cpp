#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/Core/Tabs/MenuTabDefault.hpp"
#include <External/imgui/imgui.h>

namespace CLX
{

	MenuTabDefault::MenuTabDefault(const std::string& name, const std::string& imGuiTag)
		: MainMenuTabBase(name, imGuiTag)
	{
	}

	void MenuTabDefault::Render()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu(mName.c_str()))
			{
				for (const auto& button : mButtons)
				{
					button->Render();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	MenuItemButton* MenuTabDefault::AddButton(const std::string& buttonName)
	{
		std::unique_ptr<MenuItemButton> button = std::make_unique<MenuItemButton>(buttonName);

		MenuItemButton* buttonPointer = button.get();
		buttonPointer->SetImGuiTag(std::string("##" + mName).c_str());

		mButtons.push_back(std::move(button));
		return buttonPointer;
	}

	MenuItemPopUp* MenuTabDefault::AddPopUp(const std::string& popUpName)
	{
		std::unique_ptr<MenuItemPopUp> button = std::make_unique<MenuItemPopUp>(popUpName);

		MenuItemPopUp* buttonPointer = button.get();;
		buttonPointer->SetImGuiTag(std::string("##" + mName).c_str());

		mButtons.push_back(std::move(button));
		return buttonPointer;
	}

	MenuItemMenu* MenuTabDefault::AddMenu(const std::string& menuButtonName)
	{
		std::unique_ptr<MenuItemMenu> button = std::make_unique<MenuItemMenu>(menuButtonName);

		MenuItemMenu* buttonPointer = button.get();
		buttonPointer->SetImGuiTag(std::string("##" + mName).c_str());

		mButtons.push_back(std::move(button));
		return buttonPointer;
	}

	MenuItemSelectable* MenuTabDefault::AddSelectable(const std::string& selectableButtonName)
	{
		std::unique_ptr<MenuItemSelectable> button = std::make_unique<MenuItemSelectable>(selectableButtonName);

		MenuItemSelectable* buttonPointer = button.get();
		buttonPointer->SetImGuiTag(std::string("##" + mName).c_str());

		mButtons.push_back(std::move(button));
		return buttonPointer;
	}
}
