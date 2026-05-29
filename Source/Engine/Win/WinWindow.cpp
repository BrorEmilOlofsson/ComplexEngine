#include "Engine/Precompiled/EnginePch.hpp"
#include "WinWindow.hpp"
#include "Engine/Utility/Win/WinAdapterFunctions.hpp"
#include "Engine/Utility/Win/WinUtility.hpp"
#include "Engine/Utility/Win/WinConsole.hpp"
#include "Engine/Utility/Win/WinException.hpp"

#ifdef _WIN32

#include <dwmapi.h>
#include <shellapi.h>
#include <windowsx.h>

namespace CLX
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

	Win_Window::Win_Window(const Dimension2u& windowSize, const std::wstring& name, const Win_WindowClass& windowClass, void* operatingSystem)
		: mHandle(CreateWindowImpl(GetAdjustedWindowRect(GetDefaultClientRect(windowSize), DEFAULT_WINDOW_STYLE),
			windowClass.GetInstance(), name, std::wstring(windowClass.GetName()), DEFAULT_WINDOW_STYLE, operatingSystem))
	{
		RegisterMouse(mHandle);

		enum class DWM_WINDOW_CORNER_PREFERENCE
		{
			DWMWCP_DEFAULT = 0,
			DWMWCP_DONOTROUND = 1,
			DWMWCP_ROUND = 2,
			DWMWCP_ROUNDSMALL = 3
		};

		DWM_WINDOW_CORNER_PREFERENCE preference = DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_ROUND;
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

	static Point2i HandleMouseMove(LPARAM lParam, Dimension2u windowSize)
	{
		const int xPos = GET_X_LPARAM(lParam);
		const int yPos = windowSize.GetHeight() - GET_Y_LPARAM(lParam);
		return Point2i(xPos, yPos);
	}

	bool Win_Window::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam, WindowFrameBuffer& frameBuffer)
	{

		switch (msg)
		{
		case WM_CLOSE:
			frameBuffer.hasQuit = true;
			return true;
			break;
        case WM_MOUSEMOVE:
            mWindowMousePosition = HandleMouseMove(lParam, GetClientSize());
			break;
		case WM_SIZE:
			frameBuffer.hasResized = true;
			DefWindowProc(mHandle, msg, wParam, lParam);
			break;
		case WM_DROPFILES:
		{
			HDROP hDrop = reinterpret_cast<HDROP>(wParam);
			frameBuffer.droppedFiles = GetDroppedFiles(hDrop);
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

	void Win_Window::BeginFrame(const WindowFrameBuffer& frameBuffer)
	{
		if (mHasCustomResized)
		{
			if (mResizeBuffer.fullScreen)
			{
				if (!mIsFullscreen)
				{
					mResizeBuffer.windowedRect = GetBounds();
					mResizeBuffer.windowedStyle = GetWindowStyleCustom(mHandle);
				}
				SetWindowFullscreen(mHandle);
				mIsFullscreen = true;
			}
			else if (mResizeBuffer.windowedFullScreen)
			{
				if (mIsFullscreen)
				{
					SetWindowStyle(mHandle, mResizeBuffer.windowedStyle);
					SetWindowRect(mHandle, mResizeBuffer.windowedRect, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
					mIsFullscreen = false;
				}

				::ShowWindow(mHandle, SW_MAXIMIZE);
			}
			else
			{
				if (IsZoomed(mHandle))
				{
					::ShowWindow(mHandle, SW_RESTORE);
				}
				SetWindowStyle(mHandle, mResizeBuffer.windowedStyle);
				SetWindowRect(mHandle, mResizeBuffer.windowedRect, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
				mIsFullscreen = false;
			}

			mHasCustomResized = false;
		}

		if (mIsCursorCaptured)
		{
			SetCursorPosition(mCapturedCursorPosition);
		}

		if (frameBuffer.hasResized)
		{
			mGraphicsWindowView->OnResize();
		}

		mGraphicsWindowView->BeginFrame();
	}

	void Win_Window::EndFrame(RenderContext* renderContext)
	{
		try
		{
			mGraphicsWindowView->EndFrame(renderContext);
		}
		catch (const WinException& exception)
		{
			HandleWinException(mHandle, exception);
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
		mResizeBuffer.windowedFullScreen = false;
		mHasCustomResized = true;
	}

    void Win_Window::SetTitle(const std::wstring& title)
    {
        SetWindowText(mHandle, title.c_str());
    }

	void Win_Window::SetSize(const WindowSizeSettings& sizeSettings)
	{
		if (std::holds_alternative<FullScreen>(sizeSettings))
		{
			mResizeBuffer.fullScreen = true;
			mResizeBuffer.windowedFullScreen = false;
		}
        else if (std::holds_alternative<Dimension2u>(sizeSettings))
		{
			const Dimension2u windowSize = std::get<Dimension2u>(sizeSettings);
			mResizeBuffer.fullScreen = false;
			mResizeBuffer.windowedFullScreen = false;
			const AABB2i currentClientRect = GetClientBounds();
			const Vector2f scale = static_cast<Vector2f>(ToVector2(windowSize)) / static_cast<Vector2f>(currentClientRect.GetExtent());
			const AABB2i scaledClientAABB = ScaleAABB(currentClientRect, scale);
			mResizeBuffer.windowedRect = GetAdjustedWindowRect(scaledClientAABB, GetStyle());
			mResizeBuffer.windowedStyle = GetStyle();
		}
        else if (std::holds_alternative<WindowedFullScreen>(sizeSettings))
        {
			mResizeBuffer.fullScreen = false;
			mResizeBuffer.windowedFullScreen = true;
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

	static void UpdateCapturedCursorPosition(Point2i mousePos, Dimension2u windowSize, HWND hWND, Point2i& capturedPos)
	{
		POINT mousePositionScreenSpace = ToPOINT(InvertY(mousePos, windowSize.GetHeight()));
		ClientToScreen(hWND, &mousePositionScreenSpace);

		capturedPos = ToPoint(mousePositionScreenSpace);
	}

	void Win_Window::CaptureCursor()
	{
		const RECT clipCursorRect = GetClipCursorRect(mHandle);

		ClipCursor(&clipCursorRect);
		UpdateCapturedCursorPosition(mWindowMousePosition, GetClientWindowSize(mHandle), mHandle, mCapturedCursorPosition);


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

	Dimension2u Win_Window::GetClientSize() const noexcept
	{
		return GetDimension(GetClientBounds());
	}
}

#endif
