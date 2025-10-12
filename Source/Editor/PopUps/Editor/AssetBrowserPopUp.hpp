#pragma once
#include "Editor/Core/PopUp.hpp"

namespace Simple
{
	class MenuTabWindow;
	class MenuItemPopUp;
	class NodeScriptingWindow;
}

namespace Simple
{

	class AssetBrowserPopUp final : public PopUp
	{
	public:

		AssetBrowserPopUp(const std::string& name, NodeScriptingWindow* nodeScriptWindow, MenuTabWindow* nodeScriptParentTab, MenuItemPopUp* nodeScriptButton);

		void Render(const Blackboard& blackboard) override;

	private:
		std::string mFilePopUpID;
		std::filesystem::path mFileToRemove;

		NodeScriptingWindow* mNodeScriptingWindow;
		MenuTabWindow* mNodeScriptParentTab;
		MenuItemPopUp* mNodeScriptButton;

		bool mCanOpenPopup;

		std::filesystem::path mCurrentDirectory = std::filesystem::absolute(SIMPLE_DIR_ASSETS);
	};

}