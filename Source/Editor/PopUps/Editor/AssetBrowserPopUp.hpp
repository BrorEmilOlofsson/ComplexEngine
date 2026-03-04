#pragma once
#include "Editor/Core/PopUp.hpp"

namespace CLX
{
	class MenuTabWindow;
	class MenuItemPopUp;
}

namespace CLX
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