#pragma once
#include "Editor/Core/PopUp.hpp"

namespace Simple
{
	class MenuTabWindow;
	class MenuItemPopUp;
}

namespace Simple
{

	class AssetBrowserPopUp final : public PopUp
	{
	public:

		AssetBrowserPopUp();

		void Render(const Blackboard& blackboard) override;

	private:
		std::string mFilePopUpID;
		std::filesystem::path mFileToRemove;

		MenuTabWindow* mNodeScriptParentTab;
		MenuItemPopUp* mNodeScriptButton;

		bool mCanOpenPopup = false;

		std::filesystem::path mCurrentDirectory = std::filesystem::absolute(SIMPLE_DIR_ASSETS);
	};

}