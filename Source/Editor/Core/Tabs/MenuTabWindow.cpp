#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/Core/Tabs/MenuTabWindow.hpp"
#include <External/imgui/imgui.h>

namespace Simple
{

	MenuTabWindow::MenuTabWindow(const std::string& name, const std::string& imGuiTag)
		: MainMenuTabBase(name, imGuiTag)
	{
	}

	void MenuTabWindow::Render()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu(mName.c_str()))
			{
				size_t currentButtonIndex = 0;

				for (currentButtonIndex; currentButtonIndex < mButtons.size(); ++currentButtonIndex)
				{
					auto& currentButton = mButtons[currentButtonIndex];

					if (ImGui::MenuItem(currentButton->GetName().c_str(), nullptr, &currentButton->GetIsActiveRef()))
					{
						currentButton->Invoke();
					}
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void MenuTabWindow::ActivateWindow(MenuItemPopUp* itemPopUp, const bool shouldActive)
	{
		for (size_t i = 0; i < mButtons.size(); ++i)
		{
			auto& otherButton = mButtons[i];

			otherButton->SetIsActive(false);
			otherButton->Invoke();
		}

		itemPopUp->SetIsActive(shouldActive);
		itemPopUp->Invoke();
	}

	MenuItemPopUp* MenuTabWindow::AddPopUp(const std::string& popUpButtonName)
	{
		std::unique_ptr<MenuItemPopUp> popUpButton = std::make_unique<MenuItemPopUp>(popUpButtonName);

		MenuItemPopUp* buttonPointer = popUpButton.get();
		buttonPointer->SetImGuiTag(std::string("##" + mName).c_str());

		mButtons.push_back(std::move(popUpButton));
		return buttonPointer;
	}
}