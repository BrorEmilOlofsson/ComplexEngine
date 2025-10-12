#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/Core/Items/MenuItemPopUP.hpp"

namespace Simple
{

	MenuItemPopUp::MenuItemPopUp(const std::string& name)
		: MenuItemBase(name)
		, mIsActive(false)
	{
	}

	void MenuItemPopUp::Render()
	{
		if (ImGui::MenuItem(mName.c_str(), nullptr, &mIsActive) && !mCallback.empty() && mCallback.front())
		{
			for (auto& callback : mCallback)
			{
				callback();
			}
		}
	}

	void MenuItemPopUp::AddCallback(std::function<void()>&& callback)
	{
		mCallback.push_back(std::move(callback));
	}

	void MenuItemPopUp::AddCallback(const std::function<void()>& callback)
	{
		mCallback.push_back(callback);
	}

	void MenuItemPopUp::SetIsActive(const bool shouldActive)
	{
		mIsActive = shouldActive;
	}

	bool& MenuItemPopUp::GetIsActiveRef()
	{
		return mIsActive;
	}

	void MenuItemPopUp::Invoke()
	{
		if (!mCallback.empty() && mCallback.front())
		{
			for (auto& callback : mCallback)
			{
				callback();
			}
		}
	}

}