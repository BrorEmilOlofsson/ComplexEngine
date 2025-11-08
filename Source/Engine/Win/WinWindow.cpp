#include "Engine/Precompiled/EnginePch.hpp"
#include "WinWindow.hpp"
#include "Utility/Win/WinAdapterFunctions.hpp"
#include "Utility/Algorithm.hpp"
#include <External/imgui/imgui.h>
#include <External/imgui/imgui_impl_win32.h>
#include "Utility/Win/WinUtility.hpp"
#include "Utility/Win/WinConsole.hpp"
#include "Utility/Win/WinException.hpp"

#ifdef _WIN32

#include <dwmapi.h>
#include <shellapi.h>


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Simple
{

	constexpr DWORD DEFAULT_WINDOW_STYLE = WS_OVERLAPPEDWINDOW;

	static HWND CreateWindowImpl(AABB2i rect, HINSTANCE hInstance, const std::wstring& name, const std::wstring& windowClassName, DWORD windowStyle, void* window)
	{
		return CreateWindowEx(
			0,
			windowClassName.c_str(),
			name.c_str(),
			windowStyle,
			rect.GetMin().x,
			rect.GetMin().y,
			rect.GetExtent().x,
			rect.GetExtent().y,
			nullptr,
			nullptr,
			hInstance,
			window
		);
	}

	static void RegisterMouse(HWND handle)
	{
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

		RAWINPUTDEVICE Rid = {};
		Rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
		Rid.usUsage = HID_USAGE_GENERIC_MOUSE;
		Rid.dwFlags = RIDEV_INPUTSINK;
		Rid.hwndTarget = handle;
		RegisterRawInputDevices(&Rid, 1, sizeof(RAWINPUTDEVICE));
	}

	static std::vector<std::filesystem::path> GetDroppedFiles(HDROP hDROP)
	{
		char filePath[MAX_PATH]{};

		const UINT count = DragQueryFileA(hDROP, 0, nullptr, 0);

		std::vector<std::filesystem::path> filePaths;

		for (UINT i = 0; i < count; ++i)
		{
			if (DragQueryFileA(hDROP, i, filePath, MAX_PATH))
			{
				filePaths.push_back(std::filesystem::path(filePath));
			}
		}

		return filePaths;
	}

	static void HandleWinException(HWND handle, const WinException& exception)
	{
		MessageBoxA(handle, exception.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION | MB_SETFOREGROUND);
	}

	Win_Window::Win_Window(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<AssetManager> assetManager,
		std::shared_ptr<GraphicsSettings> graphicsSettings, std::weak_ptr<DX11DepthStencilViewManager> dsvManager, std::weak_ptr<DX11SamplerState> samplerState,
		const Vector2ui& windowSize, const std::wstring& name, const Win_WindowClass& windowClass, void* operatingSystem, bool instantiateImGui)
		: mHandle(CreateWindowImpl(GetAdjustedWindowRect(GetDefaultClientRect(windowSize), DEFAULT_WINDOW_STYLE),
			windowClass.GetInstance(), name, std::wstring(windowClass.GetName()), DEFAULT_WINDOW_STYLE, operatingSystem))
		, mGraphicsWindow(device, context, this, assetManager, graphicsSettings, dsvManager, samplerState, instantiateImGui)
	{
		RegisterMouse(mHandle);

		enum class DWM_WINDOW_CORNER_PREFERENCE
		{
			DWMWCP_DEFAULT = 0,
			DWMWCP_DONOTROUND = 1,
			DWMWCP_ROUND = 2,
			DWMWCP_ROUNDSMALL = 3
		};

		DWM_WINDOW_CORNER_PREFERENCE preference = DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DONOTROUND;
		DwmSetWindowAttribute(mHandle, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(preference));

		DragAcceptFiles(mHandle, true);
	}

	Win_Window::~Win_Window()
	{
		if (mHandle)
		{
			DestroyWindow(mHandle);
		}
	}

	bool Win_Window::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(mHandle, msg, wParam, lParam))
		{
			return true;
		}

		if (mInputProcessor.HandleMessages(mHandle, msg, wParam, lParam, GetClientSize()))
		{
			return true;
		}
		switch (msg)
		{
		case WM_KILLFOCUS:
			mInputProcessor.ResetKeyStates();
			break;
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				mInputProcessor.ResetKeyStates();
			}
			break;
		case WM_SIZE:
			mFrameBuffer.hasResized = true;
			DefWindowProc(mHandle, msg, wParam, lParam);
			break;
		case WM_DROPFILES:
		{
			HDROP hDrop = reinterpret_cast<HDROP>(wParam);
			mFrameBuffer.droppedFiles = GetDroppedFiles(hDrop);
			break;
		}
		default:
			return false;
			break;
		}

		return true;
	}

	DWORD Win_Window::GetStyle() const noexcept
	{
		return GetWindowStyleCustom(mHandle);
	}

	void Win_Window::BeginFrame()
	{
		mFrameBuffer = {};

		if (mHasCustomResized)
		{
			if (mResizeBuffer.fullScreen)
			{
				mResizeBuffer.windowedRect = GetBounds();
				mResizeBuffer.windowedStyle = GetWindowStyleCustom(mHandle);
				SetWindowFullscreen(mHandle);
				mIsFullscreen = true;
			}
			else
			{
				SetWindowStyle(mHandle, mResizeBuffer.windowedStyle);
				SetWindowRect(mHandle, mResizeBuffer.windowedRect, SWP_NOZORDER | SWP_NOOWNERZORDER);
				mIsFullscreen = false;
			}

			mHasCustomResized = false;
		}

		ProcessMessages();

		mInputProcessor.Update();

		if (mIsCursorCaptured)
		{
			SetCursorPosition(mCapturedCursorPosition);
		}

		if (mFrameBuffer.hasResized)
		{
			mGraphicsWindow.OnWindowResize();
		}

		mGraphicsWindow.BeginFrame();
	}

	void Win_Window::EndFrame(RenderContext* renderContext)
	{
		try
		{
			mGraphicsWindow.EndFrame(renderContext);
		}
		catch (const WinException& exception)
		{
			HandleWinException(mHandle, exception);
		}
	}

	void Win_Window::ProcessMessages()
	{
		PROFILER_FUNCTION(profiler::colors::Cyan700);
		MSG msg = { 0 };

		while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				mFrameBuffer.hasQuit = true;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	void Win_Window::Show()
	{
		ShowWindow(mHandle, SW_SHOWDEFAULT);
	}

	void Win_Window::Hide()
	{
		ShowWindow(mHandle, SW_HIDE);
	}

	void Win_Window::ToggleFullScreen()
	{
		mResizeBuffer.fullScreen = !mIsFullscreen;
		mHasCustomResized = true;
	}


	void Win_Window::SetSize(const Vector2ui& windowSize, const bool setFullScreen)
	{
		if (setFullScreen)
		{
			mResizeBuffer.fullScreen = true;
		}
		else
		{
			AABB2i currentClientRect = GetClientBounds();
			mResizeBuffer.fullScreen = false;
			AABB2i scaledClientAABB = ScaleAABB(currentClientRect, static_cast<Vector2f>(windowSize) / static_cast<Vector2f>(currentClientRect.GetExtent()));
			mResizeBuffer.windowedRect = GetAdjustedWindowRect(scaledClientAABB, GetStyle());
			mResizeBuffer.windowedStyle = GetStyle();
		}
		mHasCustomResized = true;
	}

	void Win_Window::SetRect(const AABB2i& rect, unsigned int flags)
	{
		SetWindowRect(mHandle, rect, flags);
	}

	static RECT GetClipCursorRect(HWND hwnd)
	{
		RECT clipRect;
		GetClientRect(hwnd, &clipRect);

		POINT upperLeft = { 0 };
		upperLeft.x = clipRect.left;
		upperLeft.y = clipRect.top;

		POINT lowerRight = { 0 };
		lowerRight.x = clipRect.right;
		lowerRight.y = clipRect.bottom;

		MapWindowPoints(hwnd, nullptr, &upperLeft, 1);
		MapWindowPoints(hwnd, nullptr, &lowerRight, 1);

		clipRect.left = upperLeft.x;
		clipRect.top = upperLeft.y;
		clipRect.right = lowerRight.x;
		clipRect.bottom = lowerRight.y;

		return clipRect;
	}

	template<typename T>
	static Point2<T> InvertY(Point2<T> point, unsigned int yMax)
	{
		return Point2<T>(point.x, yMax - point.y);
	}

	static void UpdateCapturedCursorPosition(Point2i mousePos, Vector2ui windowSize, HWND hWND, Point2i& capturedPos, bool& isCaptured)
	{
		POINT mousePositionScreenSpace = ToPOINT(InvertY(mousePos, windowSize.y));
		ClientToScreen(hWND, &mousePositionScreenSpace);

		capturedPos = ToPoint(mousePositionScreenSpace);
		isCaptured = true;
	}

	void Win_Window::CaptureCursor()
	{
		const RECT clipCursorRect = GetClipCursorRect(mHandle);

		ClipCursor(&clipCursorRect);
		UpdateCapturedCursorPosition(mInputProcessor.GetInputState().GetMousePosition(), GetClientWindowSize(mHandle), mHandle, mCapturedCursorPosition, mIsCursorCaptured);


		mIsCursorCaptured = true;
	}

	void Win_Window::ReleaseCursor()
	{
		ClipCursor(nullptr);
		SetCursorPosition(mCapturedCursorPosition);
		mIsCursorCaptured = false;
	}

	void Win_Window::HandleException(const WinException& exception)
	{
		HandleWinException(mHandle, exception);
	}

	AABB2i Win_Window::GetBounds() const noexcept
	{
		return GetWindowRect(mHandle);
	}

	AABB2i Win_Window::GetClientBounds() const noexcept
	{
		return GetClientRectInScreenSpace(mHandle);
	}

	Vector2ui Win_Window::GetClientSize() const noexcept
	{
		return Vector2ui(GetClientBounds().GetExtent());
	}
}

#endif