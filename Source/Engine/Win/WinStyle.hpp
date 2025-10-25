#pragma once
#include <unordered_map>
#include <filesystem>

#ifdef _WIN32

#include "Engine/Win/WinDefines.hpp"
#include <Windows.h>
#include "Utility/Win/WinUtility.hpp"

namespace Simple
{

	class Win_Style final
	{
	public:

		Win_Style() = default;

		void LoadCursors(const std::filesystem::path& path);
		void SetCursor(const std::filesystem::path& path);
		void ShowCursor();
		void HideCursor();

		[[nodiscard]] bool IsCursorVisible() const;

	private:

		std::unordered_map<std::filesystem::path, HCURSOR> mCursors;
		bool mIsCursorVisible = true;
	};

	inline void OS_LoadCursors(Win_Style& style, const std::filesystem::path& path)
	{
		style.LoadCursors(path);
	}

	inline void OS_SetCursor(Win_Style& style, const std::filesystem::path& path)
	{
		style.SetCursor(path);
	}

	inline void OS_ShowCursor(Win_Style& style)
	{
		style.ShowCursor();
	}

	inline void OS_HideCursor(Win_Style& style)
	{
		style.HideCursor();
	}

	[[nodiscard]] inline Point2i OS_GetCursorPos(const Win_Style&)
	{
		return GetCursorPosition();
	}

	[[nodiscard]] inline bool OS_IsCursorVisible(const Win_Style& style)
	{
		return style.IsCursorVisible();
	}

	[[nodiscard]] inline void* OS_GetForegroundWindow(const Win_Style&)
	{
		return GetForegroundWindow();
	}
}

#endif