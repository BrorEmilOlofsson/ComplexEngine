#pragma once
#ifdef _WIN32

#define NOMINMAX

#include <memory>
#include <string>
#include "Engine/Math/Shapes/AABB2.hpp"
#include "WinWindowClass.hpp"
#include "Engine/OperatingSystem/WindowFrameBuffer.hpp"
#include "Engine/Graphics/GraphicsWindowView.hpp"
#include "Engine/Math/Dimension2.hpp"
#include "Engine/Utility/WindowSizeSettings.hpp"

#include "Engine/Win/WinDefines.hpp"
#include <Windows.h>
#include <WinUser.h>

namespace CLX
{

	class Win_Window final
	{
	public:

		Win_Window(
			const Dimension2u& windowSize,
			const std::wstring& name, 
			const Win_WindowClass& windowClass, 
			void* operatingSystem
		);
		~Win_Window();

		Win_Window(const Win_Window&) = delete;
		Win_Window& operator=(const Win_Window&) = delete;
		Win_Window& operator=(Win_Window&&) = delete;

		bool HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam, WindowFrameBuffer& frameBuffer);

		[[nodiscard]] HWND GetHandle() const noexcept
		{
			return mHandle;
		}

		[[nodiscard]] DWORD GetStyle() const noexcept;

		/*[[nodiscard]] const WindowFrameBuffer& GetFrameBuffer() const noexcept
		{
			return mFrameBuffer;
		}*/

		void BeginFrame(const WindowFrameBuffer& frameBuffer);
		void EndFrame(RenderContext* renderContext);

		void Show();
		void Hide();

		void SetSize(const WindowSizeSettings& sizeSettings);
		void SetRect(const AABB2i& rect, unsigned int flags);
		void ToggleFullScreen();

		void CaptureCursor();
		void ReleaseCursor();
		void HandleException(const class WinException& exception);

		[[nodiscard]] AABB2i GetBounds() const noexcept;
		[[nodiscard]] AABB2i GetClientBounds() const noexcept;

		[[nodiscard]] Dimension2u GetClientSize() const noexcept;

		[[nodiscard]] GraphicsWindowView& GetGraphicsWindow() noexcept
		{
			return *mGraphicsWindowView;
		}

		[[nodiscard]] const GraphicsWindowView& GetGraphicsWindow() const noexcept
		{
			return *mGraphicsWindowView;
		}

		void SetGraphicsWindowView(GraphicsWindowView& window)
		{
			mGraphicsWindowView = std::make_unique<GraphicsWindowView>(window);
		}

	private:
		
		HWND mHandle{};
        std::unique_ptr<GraphicsWindowView> mGraphicsWindowView;

		//WindowFrameBuffer mFrameBuffer;

		bool mIsCursorCaptured = false;
		Point2i mCapturedCursorPosition;
        Point2i mWindowMousePosition;

		bool mIsFullscreen = false;

		struct ResizeBuffer
		{
			AABB2i windowedRect;
			DWORD windowedStyle{};
			bool fullScreen = false;
		};

		ResizeBuffer mResizeBuffer;
		bool mHasCustomResized = false;
	};

	/*inline void Window_BeginFrame(Win_Window& window)
	{
		window.BeginFrame();
	}*/

	/*inline void Window_EndFrame(Win_Window& window, RenderContext* renderContext)
	{
		window.EndFrame(renderContext);
	}*/

	inline void Window_SetSize(Win_Window& window, const WindowSizeSettings& sizeSettings)
	{
		window.SetSize(sizeSettings);
	}

	inline void Window_ToggleFullScreen(Win_Window& window)
	{
		window.ToggleFullScreen();
	}

	[[nodiscard]] inline void* GetWindowHandle(const Win_Window& window) noexcept
	{
		return window.GetHandle();
	}

	[[nodiscard]] inline AABB2i GetWindowBounds(const Win_Window& window) noexcept
	{
		return window.GetBounds();
	}

	[[nodiscard]] inline AABB2i GetClientWindowBounds(const Win_Window& window) noexcept
	{
		return window.GetClientBounds();
	}

	//[[nodiscard]] inline const WindowFrameBuffer& GetWindowFrameBuffer(const Win_Window& window) noexcept
	//{
	//	return window.GetFrameBuffer();
	//}

	/*[[nodiscard]] inline const InputState& GetWindowInputState(const Win_Window& window) noexcept
	{
		return window.GetInputManager().GetInputState();
	}*/

	inline void ShowWindow(Win_Window& window)
	{
		window.Show();
	}

	inline void HideWindow(Win_Window& window)
	{
		window.Hide();
	}

	[[nodiscard]] inline void Window_CaptureCursor(Win_Window& window) noexcept
	{
		return window.CaptureCursor();
	}

	[[nodiscard]] inline void Window_ReleaseCursor(Win_Window& window) noexcept
	{
		return window.ReleaseCursor();
	}
}

#endif