#pragma once
#include "Utility/Shapes/AABB2.hpp"
#include "Engine/Win/WinInputProcessor.hpp"
#include "WinWindowClass.hpp"
#include "Engine/OperatingSystem/WindowFrameBuffer.hpp"
#include "Graphics/DX11/DX11Window.hpp"
#include "Utility/Asset/AssetLoader.hpp"
#include <memory>
#include <string>

#ifdef _WIN32

#include "Engine/Win/WinDefines.hpp"
#include <Windows.h>
#include <WinUser.h>

namespace Simple
{

	class Win_Window final
	{
	public:

		Win_Window(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<AssetManager> assetManager,
			std::shared_ptr<GraphicsSettings> graphicsSettings, std::weak_ptr<DX11DepthStencilViewManager> dsvManager, std::weak_ptr<DX11SamplerState> samplerState, const Vector2ui& windowSize, const std::wstring& name, const Win_WindowClass& windowClass, void* operatingSystem, bool instantiateImGui);
		~Win_Window();

		Win_Window(const Win_Window&) = delete;
		Win_Window& operator=(const Win_Window&) = delete;
		Win_Window& operator=(Win_Window&&) = delete;

		bool HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

		[[nodiscard]] HWND GetHandle() const noexcept
		{
			return mHandle;
		}

		[[nodiscard]] DWORD GetStyle() const noexcept;

		[[nodiscard]] Win_InputProcessor& GetInputManager() noexcept
		{
			return mInputProcessor;
		}

		[[nodiscard]] const Win_InputProcessor& GetInputManager() const noexcept
		{
			return mInputProcessor;
		}

		[[nodiscard]] const WindowFrameBuffer& GetFrameBuffer() const noexcept
		{
			return mFrameBuffer;
		}

		void BeginFrame();
		void EndFrame(RenderContext* renderContext);

		void ProcessMessages();

		void Show();
		void Hide();

		void SetSize(const Vector2ui& windowSize, const bool fullScreen);
		void SetRect(const AABB2i& rect, unsigned int flags);
		void ToggleFullScreen();

		void CaptureCursor();
		void ReleaseCursor();
		void HandleException(const class WinException& exception);

		[[nodiscard]] AABB2i GetBounds() const noexcept;
		[[nodiscard]] AABB2i GetClientBounds() const noexcept;

		[[nodiscard]] Vector2ui GetClientSize() const noexcept;

		[[nodiscard]] DX11Window& GetGraphicsWindow() noexcept
		{
			return mGraphicsWindow;
		}
		[[nodiscard]] const DX11Window& GetGraphicsWindow() const noexcept
		{
			return mGraphicsWindow;
		}
	private:
		
		HWND mHandle{};
		Win_InputProcessor mInputProcessor;
		DX11Window mGraphicsWindow;

		WindowFrameBuffer mFrameBuffer;

		bool mIsCursorCaptured = false;
		Point2i mCapturedCursorPosition;

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

	inline void Window_BeginFrame(Win_Window& window)
	{
		window.BeginFrame();
	}

	inline void Window_EndFrame(Win_Window& window, RenderContext* renderContext)
	{
		window.EndFrame(renderContext);
	}

	/*inline void Window_Update(Win_Window& window, RenderList& renderList)
	{
		window.Update(renderList);
	}*/

	/*inline void Window_Render(Win_Window& window, const RenderState& renderState)
	{
		window.Render(renderState);
	}*/

	inline void Window_SetSize(Win_Window& window, Vector2ui size, const bool fullScreen)
	{
		window.SetSize(size, fullScreen);
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

	[[nodiscard]] inline const WindowFrameBuffer& GetWindowFrameBuffer(const Win_Window& window) noexcept
	{
		return window.GetFrameBuffer();
	}

	[[nodiscard]] inline const InputState& GetWindowInputState(const Win_Window& window) noexcept
	{
		return window.GetInputManager().GetInputState();
	}

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