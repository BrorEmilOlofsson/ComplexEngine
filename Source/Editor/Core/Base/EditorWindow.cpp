#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorWindow.hpp"
#include "Engine/Utility/Algorithm.hpp"

namespace CLX
{
	EditorWindow::EditorWindow(const std::string& name, const std::string& tag)
		: mName(name)
		//, mIsActive(true)
		, mIsFocus(false)
	{
		mImGuiTag = "##" + ConvertTypeIndexNameToPrettyName(tag);
		mImGuiName = mName + mImGuiTag;
	}

	void EditorWindow::SetWindowName(const std::string& newName)
	{
		mName = newName;
		mImGuiName = mName + mImGuiTag;
	}

	void EditorWindow::SetImGuiTag(const std::string& newTag)
	{
		mImGuiTag = newTag;
		mImGuiName = mName + mImGuiTag;
	}

	bool EditorWindow::IsActive() const
	{
		return mIsActive;
	}

	bool EditorWindow::IsFocused() const
	{
		return mIsFocus;
	}

	bool& EditorWindow::IsActive()
	{
		return mIsActive;
	}

	bool& EditorWindow::IsFocused()
	{
		return mIsFocus;
	}

	const std::string& EditorWindow::GetWindowName() const
	{
		return mName;
	}

	const std::string& EditorWindow::GetImGuiName() const
	{
		return mImGuiName;
	}
}
