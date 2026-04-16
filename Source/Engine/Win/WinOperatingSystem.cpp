#include "Engine/Precompiled/EnginePch.hpp"
#ifdef _WIN32

#include "WinOperatingSystem.hpp"
#include <External/imgui/imgui.h>
#include "Engine/Utility/Win/WinException.hpp"
#include "Engine/Graphics/DX11/DX11Foundation.hpp"
#include <set>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace CLX
{
    static LRESULT HandleMsgMain(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        Win_OperatingSystem* const operatingSystem = reinterpret_cast<Win_OperatingSystem*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (operatingSystem == nullptr)
        {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        return operatingSystem->HandleMessage(hwnd, msg, wParam, lParam);
    }

    static LRESULT HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (msg == WM_NCCREATE)
        {
            const CREATESTRUCTW* const create = reinterpret_cast<const CREATESTRUCTW*>(lParam);
            Win_OperatingSystem* const operatingSystem = static_cast<Win_OperatingSystem*>(create->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(operatingSystem));
            SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgMain));
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    Win_OperatingSystem::Win_OperatingSystem(HINSTANCE instanceHandle, std::wstring className)
        : mInstanceHandle(instanceHandle)
        , mWindowClass(std::make_unique<Win_WindowClass>(instanceHandle, std::move(className), HandleMsgSetup))
        , mGraphicsFoundation(DX11Foundation())
    {}

    Win_OperatingSystem::Win_OperatingSystem(Win_OperatingSystem&& other)
        : mInstanceHandle(std::move(other.mInstanceHandle))
        , mWindowClass(std::move(other.mWindowClass))
        , mGraphicsFoundation(std::move(other.mGraphicsFoundation))
        , mWindows(std::move(other.mWindows))
        , mCursors(std::move(other.mCursors))
        , mIsCursorVisible(std::move(other.mIsCursorVisible))
    {
        for (auto& window : mWindows)
        {
            SetWindowLongPtr(window->GetHandle(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        }
    }

    void Win_OperatingSystem::Init()
    {
        mGraphicsFoundation.Init();
    }

    void Win_OperatingSystem::Shutdown()
    {
        mGraphicsFoundation.Shutdown();
    }

    void Win_OperatingSystem::BeginFrame(const GraphicsBufferData& data)
    {
        mGraphicsFoundation.BeginFrame(data);
        for (auto& window : mWindows)
        {
            window->BeginFrame();
        }
        mInputProcessor.Update();
    }

    void Win_OperatingSystem::EndFrame(RenderContext* renderContext)
    {
        for (auto& window : mWindows)
        {
            window->GetGraphicsWindow().BindBackBuffer();
        }
        mGraphicsFoundation.EndFrame();
        for (auto& window : mWindows)
        {
            window->EndFrame(renderContext);
        }
    }

    LRESULT Win_OperatingSystem::HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        static uint32_t counter = 0;
        static std::set<HWND> hwnds;
        hwnds.insert(hwnd);

        if (hwnds.size() > 1)
        {
            // Not getting in here.
            std::println("Multiple HWNDs detected: {}", hwnds.size());
        }
        if (msg == WM_LBUTTONDOWN)
        {
            // I am not getting LBUTTONDOWN events here if I click the popup
            std::println("Count: {}", counter);
            counter++;
        }
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        {
            return 1;
        }

        switch (msg)
        {
        case WM_SETCURSOR:

            if (LOWORD(lParam) == HTCLIENT)
            {
                ::SetCursor(LoadCursor(nullptr, IDC_ARROW));
                return TRUE;
            }

            return DefWindowProc(hwnd, msg, wParam, lParam);

            break;
        case WM_DESTROY:
            PostQuitMessage(0);

            break;
        case WM_KILLFOCUS:
            mInputProcessor.ResetKeyStates();
            break;
        case WM_ACTIVATE:
            if (LOWORD(wParam) == WA_INACTIVE)
            {
                mInputProcessor.ResetKeyStates();
            }
            break;
        default:
            auto it = std::ranges::find_if(mWindows, [hwnd](auto& window) { return hwnd == window->GetHandle(); });

            if (it != end(mWindows))
            {
                if (it->get()->HandleMessage(msg, wParam, lParam))
                {
                    return 0;
                }
            }

            if (mInputProcessor.HandleMessage(hwnd, msg, wParam, lParam))
            {
                return 0;
            }

            return DefWindowProc(hwnd, msg, wParam, lParam);
            break;
        }

        return 0;
    }

    uint32_t Win_OperatingSystem::MakeWindow(Dimension2u size, std::wstring title)
    {
        try
        {
            auto window = std::make_unique<Win_Window>(
                size,
                title,
                *mWindowClass,
                this
            );
            GraphicsWindowView windowView = mGraphicsFoundation.MakeWindow(WindowView(*window));
            window->SetGraphicsWindowView(windowView);
            SetWindowLongPtr(window->GetHandle(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));


            window->GetGraphicsWindow().Init();

            mWindows.push_back(std::move(window));
            return static_cast<uint32_t>(mWindows.size() - 1);
        }
        catch (const WinException& exception)
        {
            if (!mWindows.empty())
            {
                mWindows.front()->HandleException(exception);
            }
            else
            {
                assert(false);
            }
        }

        ASSERT(false);
        return std::numeric_limits<uint32_t>::max();
    }

    Win_Window& Win_OperatingSystem::GetWindow(const WindowID windowID)
    {
        return *mWindows[windowID];
    }

    const Win_Window& Win_OperatingSystem::GetWindow(const WindowID windowID) const
    {
        return *mWindows[windowID];
    }

    GraphicsFoundation& Win_OperatingSystem::GetGraphicsFoundation() noexcept
    {
        return mGraphicsFoundation;
    }

    const GraphicsFoundation& Win_OperatingSystem::GetGraphicsFoundation() const noexcept
    {
        return mGraphicsFoundation;
    }

    const InputState& Win_OperatingSystem::GetInputState() const noexcept
    {
        return mInputProcessor.GetInputState();
    }

    void Win_OperatingSystem::LoadCursors(const std::filesystem::path& path)
    {
        mCursors.emplace("DefaultCursor", LoadCursorW(nullptr, IDC_ARROW));
        assert(mCursors["DefaultCursor"] && "Failed to load Custom Cursor");

        for (const std::filesystem::path& cursorPath : std::filesystem::directory_iterator(std::filesystem::absolute(path)))
        {
            const std::filesystem::path name = cursorPath.filename();
            const HCURSOR cursor = LoadCursorFromFile(cursorPath.c_str());

            assert(cursor != nullptr && "Failed to load Custom Cursor");

            mCursors.emplace(name, cursor);
        }
    }


    void Win_OperatingSystem::SetCursor(const std::filesystem::path& path)
    {
        HCURSOR cursor = mCursors.at(path);
        ::SetCursor(cursor);
    }

    void Win_OperatingSystem::ShowCursor()
    {
        ::ShowCursor(true);
        mIsCursorVisible = true;
    }

    void Win_OperatingSystem::HideCursor()
    {
        ::ShowCursor(false);
        mIsCursorVisible = false;
    }

    bool Win_OperatingSystem::IsCursorVisible() const
    {
        return mIsCursorVisible;
    }

}

#endif