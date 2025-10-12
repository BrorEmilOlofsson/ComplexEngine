#pragma once
#include "Engine/Win/WinStyle.hpp"
#include "Engine/Win/WinWindow.hpp"
#include "Graphics/DX11/DX11Foundation.hpp"
#include "Engine/OperatingSystem/WindowHandle.hpp"
#include <string>
#include <memory>

#ifdef _WIN32

namespace Simple
{

	using WindowID = unsigned int;

	class Win_OperatingSystem final
	{
	public:

		explicit Win_OperatingSystem(HINSTANCE instanceHandle, std::wstring className);

		Win_OperatingSystem(const Win_OperatingSystem&) = delete;
		Win_OperatingSystem(Win_OperatingSystem&&);

		void Init();
		void Shutdown();
		void BeginFrame(const GraphicsBufferData& bufferData);
		void EndFrame(std::optional<RenderTargetView> renderTarget);
		void Render();
		void Render(const RenderState& renderState);

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

		[[nodiscard]] DX11Foundation& GetGraphicsFoundation() noexcept
		{
			return mGraphicsFoundation;
		}
		[[nodiscard]] const DX11Foundation& GetGraphicsFoundation() const noexcept
		{
			return mGraphicsFoundation;
		}

	private:

		HINSTANCE mInstanceHandle;
		std::unique_ptr<Win_WindowClass> mWindowClass;
		DX11Foundation mGraphicsFoundation;
		std::vector<std::unique_ptr<Win_Window>> mWindows;
		Win_Style mStyle;
		std::shared_ptr<AssetManager> mAssetManager;
		std::shared_ptr<GraphicsSettings> mGraphicsSettings;
	};

	inline void OSBeginFrame(Win_OperatingSystem& os, const GraphicsBufferData& data)
	{
		os.BeginFrame(data);
	}

	inline void OSEndFrame(Win_OperatingSystem& os, std::optional<RenderTargetView> renderTarget)
	{
		os.EndFrame(renderTarget);
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

	inline void OSRender(Win_OperatingSystem& os, const RenderState& renderState)
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

	[[nodiscard]] inline RenderTargetView OSCreateRenderTarget(Win_OperatingSystem& os, const Vector2ui& size)
	{
		return os.GetGraphicsFoundation().CreateRenderTarget(size);
	}

	[[nodiscard]] inline DepthStencilViewHandle OSCreateDepthStencilView(Win_OperatingSystem& os, const Vector2ui& size)
	{
		return os.GetGraphicsFoundation().CreateDepthStencilView(size);
	}

	inline void OSSetAssetManager(Win_OperatingSystem& os, std::shared_ptr<AssetManager> assetManager)
	{
		os.SetAssetManager(assetManager);
	}

	inline void OSSetGraphicsSettings(Win_OperatingSystem& os, std::shared_ptr<GraphicsSettings> graphicsSettings)
	{
		os.SetGraphicsSettings(graphicsSettings);
	}
}

#endif