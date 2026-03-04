#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11ImGuiWindow.hpp"
#include <External/imgui/imgui_impl_win32.h>

namespace CLX
{
	DX11ImGuiWindow::DX11ImGuiWindow(HWND hwnd)
		: mResource(Make(hwnd))
	{
	}
	
}