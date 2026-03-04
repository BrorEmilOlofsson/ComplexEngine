#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/Core/Base/Window.hpp"
#include "Engine/Utility/Algorithm.hpp"

namespace Simple
{
	Window::Window(const std::string& name, const std::string& tag)
		: mName(name)
		//, mIsActive(true)
		, mIsFocus(false)
	{
		mImGuiTag = "##" + Simple::ConvertTypeIndexNameToPrettyName(tag);
		mImGuiName = mName + mImGuiTag;
	}

	void Window::SetWindowName(const std::string& newName)
	{
		mName = newName;
		mImGuiName = mName + mImGuiTag;
	}

	void Window::SetImGuiTag(const std::string& newTag)
	{
		mImGuiTag = newTag;
		mImGuiName = mName + mImGuiTag;
	}

	bool Window::IsActive() const
	{
		return mIsActive;
	}

	bool Window::IsFocused() const
	{
		return mIsFocus;
	}

	bool& Window::IsActive()
	{
		return mIsActive;
	}

	bool& Window::IsFocused()
	{
		return mIsFocus;
	}

	const std::string& Window::GetWindowName() const
	{
		return mName;
	}

	const std::string& Window::GetImGuiName() const
	{
		return mImGuiName;
	}
}
