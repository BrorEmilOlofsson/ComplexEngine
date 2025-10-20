#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11Window.hpp"
#include "Utility/Algorithm.hpp"
#include "Graphics/DX11/DX11Factory.hpp"
#include "Utility/Win/WinException.hpp"
#include "Engine/Win/WinWindow.hpp"
#include "Utility/Win/WinUtility.hpp"
#include "Utility/ShapeMath.hpp"
#include "Graphics/DX11/Texture/DX11Texture.hpp"
#include "Utility/Asset/AssetManager.hpp"
#include "Engine/OperatingSystem/WindowView.hpp"
#include "Graphics/Texture/Texture.hpp"
#include "Graphics/DX11/RenderTarget/DX11RenderTarget.hpp"
#include <External/imgui/imgui.h>
#include <External/nlohmann/json.hpp>
#include <fstream>

#ifdef _DEBUG
#define REPORT_DX_WARNINGS
#endif

namespace Simple
{

	void LoadSettingsFromJson(bool& vSync)
	{
		const std::filesystem::path filename = std::filesystem::absolute(SIMPLE_SETTINGS_GAME);

		std::ifstream file(filename);
		if (!file.is_open())
		{
			return;
		}

		const nlohmann::json json = nlohmann::json::parse(file);
		file.close();

		vSync = json["Game_Settings"]["VSync"];
	}

	std::optional<DX11ImGuiWindow> CreateImGuiWindow(const bool instantiate, HWND handle)
	{
		if (instantiate)
		{
			return std::make_optional<DX11ImGuiWindow>(handle);
		}

		return std::nullopt;
	}

	DX11Window::DX11Window(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,
		Win_Window* window, std::shared_ptr<AssetManager> assetManager, std::shared_ptr<GraphicsSettings> graphicsSettings,
		std::weak_ptr<DX11DepthStencilViewManager> dsvManager, bool instantiateImGui)
		: mDevice(device)
		, mContext(context)
		, mSwapChain(DX11Factory::CreateSwapChain(window->GetHandle(), window->GetClientSize(), device))
		, mWindow(window)
		, mAssetManager(assetManager)
		, mGraphicsSettings(graphicsSettings)
		, mBackBuffer(context, device, *DX11Factory::GetBackBuffer(*mSwapChain.Get()).Get(), window->GetClientSize(), false)
		, mImGuiWindow(CreateImGuiWindow(instantiateImGui, window->GetHandle()))
		, mDepthStencilViewManager(dsvManager)
	{
	}

	void DX11Window::Init()
	{
		LoadSettingsFromJson(mGraphicsSettings.lock()->vSync);

		{
			const Vector2ui windowSize = mWindow->GetClientSize();

			const D3D11_VIEWPORT viewport = DX11Factory::CreateViewport(windowSize);

			mContext->RSSetViewports(1, &viewport);

			{
				//auto backBuffer = ;

				//mBackBuffer.InitRenderTargetView(*backBuffer.Get(), windowSize);

				/*{
					mIDTexture = DX11Factory::CreateRenderTargetTexture(*mDevice.Get(), DX11Factory::CreateObjectSelectionTextureDesc(windowSize));
					mObjectIDRenderTarget.Init(*mDevice.Get(), *mIDTexture.Get(), windowSize);
				}*/
				/*{

					D3D11_TEXTURE2D_DESC desc = {};
					mIDTexture->GetDesc(&desc);

					desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
					desc.Usage = D3D11_USAGE_STAGING;
					desc.BindFlags = 0;
					desc.MiscFlags = 0;

					HRESULT hr = mDevice->CreateTexture2D(&desc, nullptr, &mStagingTexture);
					WIN_CHECK_HRESULT(hr);
				}*/
			}

			
			mRasterizerState = DX11Factory::CreateRasterizerState_BackfaceCulling(*mDevice.Get());
		}

		mDepthStencilViewHandle = mDepthStencilViewManager.lock()->Create(mWindow->GetClientSize());

		mContext->RSSetState(mRasterizerState.Get());
	}

	void DX11Window::BeginFrame()
	{
		PrepareFrame();
		mBackBuffer.Clear(mGraphicsSettings.lock()->clearColor);
	}

	void DX11Window::EndFrame(RenderContext* renderContext)
	{
		PROFILER_FUNCTION(profiler::colors::Indigo800);

		BindBackBuffer();

		if (renderContext)
		{
			RenderFullScreen(
				*mContext.Get(),
				static_cast<DX11RenderTarget*>(renderContext->GetOutputRenderTarget())->GetShaderResourceView(),
				*mSamplerState.lock(),
				mAssetManager.lock()->GetVertexShader(GetPath(eVertexShaderType::FullScreen)),
				mAssetManager.lock()->GetPixelShader(GetPath(ePixelShaderType::FullScreen))
			);
		}

		PROFILER_BEGIN("Present Frame");
		const UINT syncInterval = mGraphicsSettings.lock()->vSync ? 1 : 0;
		const HRESULT result = mSwapChain->Present(syncInterval, 0);
		WIN_CHECK_HRESULT(result);
		PROFILER_END();
	}

	void DX11Window::PrepareFrame()
	{
	}

	void DX11Window::OnWindowResize()
	{
		const Vector2ui windowSize = mWindow->GetClientSize();

		mBackBuffer.GetRenderTargetView()->Release();

		const HRESULT result = mSwapChain->ResizeBuffers(0, windowSize.x, windowSize.y, DXGI_FORMAT_UNKNOWN, 0);
		WIN_CHECK_HRESULT(result);

		auto backBuffer = DX11Factory::GetBackBuffer(*mSwapChain.Get());
		mBackBuffer.InitRenderTargetView(*backBuffer.Get(), windowSize);

		{
			//auto viewport = DX11Factory::CreateViewport(windowSize);
			//mContext->RSSetViewports(1, &viewport);
		}
	}

	void DX11Window::BindBackBuffer()
	{
		mBackBuffer.Set(mDepthStencilViewManager.lock()->Get(mDepthStencilViewHandle).Get());
	}
}
