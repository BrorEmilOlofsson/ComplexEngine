#pragma once
#include <string>

#ifdef _WIN32

#include "Engine/Win/WinDefines.hpp"
#include <Windows.h>


namespace CLX
{

	class Win_WindowClass final
	{
	public:

		Win_WindowClass(HINSTANCE hInstance, std::wstring name, LRESULT(*aWinProc)(HWND, UINT, WPARAM, LPARAM));
		~Win_WindowClass();

		[[nodiscard]] HINSTANCE GetInstance() const;
		[[nodiscard]] std::wstring_view GetName() const;

	private:

		HINSTANCE mInstance{};
		std::wstring mName;
	};
}

#endif