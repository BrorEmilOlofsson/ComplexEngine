#include "Engine/Precompiled/EnginePch.hpp"
#include "WinOperatingSystem.hpp"
#include "Utility/Win/WinException.hpp"
#include "Utility/Asset/AssetManager.hpp"

#ifdef _WIN32

namespace Simple
{
	static LRESULT HandleMsgMain(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Win_OperatingSystem* const operatingSystem = reinterpret_cast<Win_OperatingSystem*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (operatingSystem == nullptr)
		{
			return 0;
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
		, mWindowClass(std::make_unique<Win_WindowClass>(instanceHandle, className, HandleMsgSetup))
        , mGraphicsFoundationNew(DX11Foundation())
	{
	}

	Win_OperatingSystem::Win_OperatingSystem(Win_OperatingSystem&& other)
		: mInstanceHandle(std::move(other.mInstanceHandle))
		, mWindowClass(std::move(other.mWindowClass))
        , mGraphicsFoundationNew(std::move(other.mGraphicsFoundationNew))
		//, mGraphicsFoundation(std::move(other.mGraphicsFoundation))
		, mWindows(std::move(other.mWindows))
		, mStyle(std::move(other.mStyle))
		, mAssetManager(std::move(other.mAssetManager))
		, mGraphicsSettings(std::move(other.mGraphicsSettings))
	{
		for (auto& window : mWindows)
		{
			SetWindowLongPtr(window->GetHandle(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		}
	}

	void Win_OperatingSystem::SetAssetManager(std::shared_ptr<AssetManager> assetManager)
	{
		mAssetManager = std::move(assetManager);
		mGraphicsFoundationNew.SetAssetManager(mAssetManager);
		//mGraphicsFoundation.SetAssetManager(mAssetManager);
	}

	void Win_OperatingSystem::SetGraphicsSettings(std::shared_ptr<GraphicsSettings> graphicsSettings)
	{
		mGraphicsSettings = graphicsSettings;
		mGraphicsFoundationNew.SetGraphicsSettings(graphicsSettings);
		//mGraphicsFoundation.SetGraphicsSettings(graphicsSettings);
	}

	void Win_OperatingSystem::Init()
	{
		mGraphicsFoundationNew.Init();
		//mGraphicsFoundation.Init();
	}

	void Win_OperatingSystem::Shutdown()
	{
		mGraphicsFoundationNew.Shutdown();
		//mGraphicsFoundation.Shutdown();
	}

	void Win_OperatingSystem::BeginFrame(const GraphicsBufferData& data)
	{
		mGraphicsFoundationNew.BeginFrame(data);
		//mGraphicsFoundation.BeginFrame(data);
		for (auto& window : mWindows)
		{
			window->BeginFrame();
		}
	}

	void Win_OperatingSystem::EndFrame(RenderContext* renderContext)
	{
		for (auto& window : mWindows)
		{
			window->GetGraphicsWindow().BindBackBuffer();
		}
		mGraphicsFoundationNew.EndFrame();
		//mGraphicsFoundation.EndFrame();
		for (auto& window : mWindows)
		{
			window->EndFrame(renderContext);
		}
	}

	void Win_OperatingSystem::Render()
	{

	}

	void Win_OperatingSystem::Render(RenderState& renderState)
	{
		mGraphicsFoundationNew.Render(renderState);
		//mGraphicsFoundation.Render(renderState);
	}

	uint32_t Win_OperatingSystem::MakeWindow(Vector2ui size, std::wstring title)
	{
		try
		{


			auto window = std::make_unique<Win_Window>(
				/*mGraphicsFoundation.GetDevice(),
				mGraphicsFoundation.GetContext(),
				mAssetManager,
				mGraphicsSettings,
				mGraphicsFoundation.GetDepthStencilViewManager(),
				mGraphicsFoundation.GetSamplerState(),*/
				size,
				title,
				*mWindowClass,
				this//,
				//mWindows.empty()
			);
			GraphicsWindowView windowView = mGraphicsFoundationNew.MakeWindow(WindowView(*window));
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
		}

		return std::numeric_limits<uint32_t>::max();
	}

	LRESULT Win_OperatingSystem::HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_SETCURSOR:

			if (LOWORD(lParam) == HTCLIENT)
			{
				SetCursor(LoadCursor(nullptr, IDC_ARROW));
				return TRUE;
			}

			return DefWindowProc(hwnd, msg, wParam, lParam);

			break;
		case WM_DESTROY:
			PostQuitMessage(0);
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

			return DefWindowProc(hwnd, msg, wParam, lParam);
			break;
		}

		return 0;
	}

}

#endif