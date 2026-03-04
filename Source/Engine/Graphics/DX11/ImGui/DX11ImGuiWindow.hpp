#pragma once

#ifdef _WIN32

#include <Windows.h>
#include <windef.h>
#include <exception>
#include "Engine/Utility/FunctionResource.hpp"
#include <External/imgui/imgui_impl_win32.h>

namespace Simple
{

    class DX11ImGuiWindow final
    {
    public:

        explicit DX11ImGuiWindow(HWND hwnd);

        DX11ImGuiWindow(const DX11ImGuiWindow&) = delete;
        DX11ImGuiWindow& operator=(const DX11ImGuiWindow&) = delete;
        DX11ImGuiWindow(DX11ImGuiWindow&&) = default;
        DX11ImGuiWindow& operator=(DX11ImGuiWindow&&) = default;

        ~DX11ImGuiWindow() = default;

    private:

        static void Create(HWND hwnd)
        {
            if (!ImGui_ImplWin32_Init(hwnd))
            {
                throw std::exception("Could not initialize imgui win32");
            }
        }

        static void Destroy()
        {
            ImGui_ImplWin32_Shutdown();
        }

        static FunctionResource<decltype(&Create), decltype(&Destroy)> Make(HWND hwnd)
        {
            return FunctionResource{ &Create, &Destroy, hwnd };
        }

        using ResourceType = decltype(Make(HWND{}));
        ResourceType mResource;
    };
}

#endif