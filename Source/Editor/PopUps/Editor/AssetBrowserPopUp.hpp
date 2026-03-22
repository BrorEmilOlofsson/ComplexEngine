#pragma once
#include "Editor/Core/PopUp.hpp"
#include "Engine/Utility/Blackboard.hpp"
#include "Engine/Utility/File/FileUtility.hpp"

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

		MenuTabWindow* mNodeScriptParentTab = nullptr;
		MenuItemPopUp* mNodeScriptButton = nullptr;

		bool mCanOpenPopup = false;

		std::filesystem::path mCurrentDirectory = GetAbsoluteAssetPath();

        std::string mRenameBuffer;
	};

}