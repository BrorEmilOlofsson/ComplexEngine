#pragma once
#include "Engine/Win/WinStyle.hpp"
#include "Engine/Win/WinWindow.hpp"
#include "Graphics/DX11/DX11Foundation.hpp"
#include "Engine/OperatingSystem/WindowHandle.hpp"
#include "Graphics/GraphicsFoundation.hpp"
#include <string>
#include <memory>

#ifdef _WIN32

#include "Engine/Win/WinDefines.hpp"
#include <Windows.h>

namespace Simple
{

	using WindowID = unsigned int;

	class Win_OperatingSystem final
	{
	public:

		explicit Win_OperatingSystem(HINSTANCE instanceHandle, std::wstring className);

		Win_OperatingSystem(const Win_OperatingSystem&) = delete;
		Win_OperatingSystem(Win_OperatingSystem&&);
		Win_OperatingSystem& operator=(const Win_OperatingSystem&) = delete;
		Win_OperatingSystem& operator=(Win_OperatingSystem&&) = delete;

		void Init();
		void Shutdown();
		void BeginFrame(const GraphicsBufferData& bufferData);
		void EndFrame(RenderContext* renderContext);
		void Render();
		void Render(RenderState& renderState);

		[[nodiscard]] Win_Window& GetWindow(const WindowID windowID)
		{
			return *mWindows[windowID];
		}

		[[nodiscard]] const Win_Window& GetWindow(const WindowID windowID) const
		{
			return *mWindows[windowID];
		}

		[[nodiscard]] Win_Style& GetStyle()
		{
			return mStyle;
		}

		void SetAssetManager(std::shared_ptr<AssetManager> assetManager);
		void SetGraphicsSettings(std::shared_ptr<GraphicsSettings> graphicsSettings);

		unsigned int MakeWindow(Vector2ui size, std::wstring title);
		LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		[[nodiscard]] GraphicsFoundation& GetGraphicsFoundation() noexcept
		{
			return mGraphicsFoundation;
		}
		[[nodiscard]] const GraphicsFoundation& GetGraphicsFoundation() const noexcept
		{
			return mGraphicsFoundation;
		}

	private:

		HINSTANCE mInstanceHandle = HINSTANCE{};
		std::unique_ptr<Win_WindowClass> mWindowClass;
		GraphicsFoundation mGraphicsFoundation;
		std::vector<std::unique_ptr<Win_Window>> mWindows;
		Win_Style mStyle;
	};

	inline void OSBeginFrame(Win_OperatingSystem& os, const GraphicsBufferData& data)
	{
		os.BeginFrame(data);
	}

	inline void OSEndFrame(Win_OperatingSystem& os, RenderContext* renderContext)
	{
		os.EndFrame(renderContext);
	}

	inline void OSInit(Win_OperatingSystem& os)
	{
		os.Init();
	}

	inline void OSShutdown(Win_OperatingSystem& os)
	{
		os.Shutdown();
	}

	inline void OSRender(Win_OperatingSystem& os)
	{
		os.Render();
	}

	inline void OSRender(Win_OperatingSystem& os, RenderState& renderState)
	{
		os.Render(renderState);
	}

	[[nodiscard]] inline Win_Window& GetOSWindow(Win_OperatingSystem& os, WindowHandle windowHandle)
	{
		return os.GetWindow(windowHandle.id);
	}

	[[nodiscard]] inline const Win_Window& GetOSWindow(const Win_OperatingSystem& os, WindowHandle windowHandle)
	{
		return os.GetWindow(windowHandle.id);
	}

	[[nodiscard]] inline Win_Style& GetOSStyle(Win_OperatingSystem& os)
	{
		return os.GetStyle();
	}

	[[nodiscard]] inline WindowHandle OSCreateWindow(Win_OperatingSystem& os, Vector2ui size, std::wstring title)
	{
		return WindowHandle(os.MakeWindow(size, title));
	}

	[[nodiscard]] inline GraphicsFoundation& OSGetGraphicsFoundation(Win_OperatingSystem& os)
	{
		return os.GetGraphicsFoundation();
	}

	[[nodiscard]] inline const GraphicsFoundation& OSGetGraphicsFoundation(const Win_OperatingSystem& os)
	{
		return os.GetGraphicsFoundation();
    }
}

#endif