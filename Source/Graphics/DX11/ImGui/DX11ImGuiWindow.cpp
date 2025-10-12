#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11ImGuiWindow.hpp"
#include <External/imgui/imgui_impl_win32.h>

namespace Simple
{
	DX11ImGuiWindow::DX11ImGuiWindow(HWND hwnd)
	{
		if (!ImGui_ImplWin32_Init(hwnd))
		{
			throw std::exception("Could not initialize imgui win32");
		}
	}

	DX11ImGuiWindow::~DX11ImGuiWindow()
	{
		ImGui_ImplWin32_Shutdown();
	}
	
}