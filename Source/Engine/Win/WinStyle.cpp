#include "Engine/Precompiled/EnginePch.hpp"
#include "WinStyle.hpp"
#include "Utility/File/FileUtility.hpp"
#include "Utility/Algorithm.hpp"

#ifdef _WIN32

namespace Simple
{

	void Win_Style::LoadCursors(const std::filesystem::path& path)
	{
		mCursors.emplace("DefaultCursor", LoadCursorW(nullptr, IDC_ARROW));
		assert(mCursors["DefaultCursor"] && "Failed to load Custom Cursor");

		const std::vector<std::filesystem::path> absolutePaths = FileUtility::GetFilesFromDirectory(std::filesystem::absolute(path));

		for (const std::filesystem::path& cursorPath : absolutePaths)
		{
			const std::filesystem::path name = cursorPath.filename();
			const HCURSOR cursor = LoadCursorFromFile(cursorPath.c_str());

			assert(cursor != nullptr && "Failed to load Custom Cursor");

			mCursors.emplace(name, cursor);
		}
	}


	void Win_Style::SetCursor(const std::filesystem::path& path)
	{
		HCURSOR cursor = mCursors.at(path);
		::SetCursor(cursor);
	}

	void Win_Style::ShowCursor()
	{
		::ShowCursor(true);
		mIsCursorVisible = true;
	}

	void Win_Style::HideCursor()
	{
		::ShowCursor(false);
		mIsCursorVisible = false;
	}

	bool Win_Style::IsCursorVisible() const
	{
		return mIsCursorVisible;
	}

}

#endif