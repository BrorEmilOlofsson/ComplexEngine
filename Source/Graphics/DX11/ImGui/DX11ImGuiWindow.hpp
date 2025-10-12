#pragma once

#ifdef _WIN32

#include <Windows.h>
#include <windef.h>

namespace Simple
{

	class DX11ImGuiWindow final
	{
	public:

		explicit DX11ImGuiWindow(HWND hwnd);
		~DX11ImGuiWindow();

	};
}

#endif