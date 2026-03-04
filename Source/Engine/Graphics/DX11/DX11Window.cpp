#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11Window.hpp"
#include "Engine/Graphics/DX11/DX11Factory.hpp"
#include "Engine/Utility/Win/WinException.hpp"
#include "Engine/Asset/AssetManager.hpp"
#include "Engine/OperatingSystem/WindowView.hpp"
#include "Engine/Graphics/DX11/RenderTarget/DX11RenderTarget.hpp"

#ifdef _DEBUG
#define REPORT_DX_WARNINGS
#endif

namespace CLX
{

	std::optional<DX11ImGuiWindow> CreateImGuiWindow(const bool instantiate, HWND handle)
	{
		if (instantiate)
		{
			return std::make_optional<DX11ImGuiWindow>(handle);
		}

		return std::nullopt;
	}

	DX11Window::DX11Window(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,
		WindowView windowView, std::shared_ptr<AssetManager> assetManager, std::shared_ptr<GraphicsSettings> graphicsSettings,
		std::weak_ptr<DX11DepthStencilViewManager> dsvManager, std::weak_ptr<DX11SamplerState> samplerState, bool instantiateImGui)
		: mDevice(device)
		, mContext(context)
		, mSwapChain(DX11Factory::CreateSwapChain(static_cast<HWND>(windowView.GetHandle()), windowView.GetClientSize(), device))
		, mWindowView(std::move(windowView))
		, mAssetManager(assetManager)
		, mGraphicsSettings(graphicsSettings)
		, mBackBuffer(context, device, *DX11Factory::GetBackBuffer(*mSwapChain.Get()).Get(), windowView.GetClientSize(), false)
		, mImGuiWindow(CreateImGuiWindow(instantiateImGui, static_cast<HWND>(windowView.GetHandle())))
		, mDepthStencilViewManager(dsvManager)
		, mSamplerState(std::move(samplerState))
	{
	}

	void DX11Window::Init()
	{
		{
			const Vector2ui windowSize = mWindowView.GetClientSize();

			const D3D11_VIEWPORT viewport = DX11Factory::CreateViewport(windowSize);

			mContext->RSSetViewports(1, &viewport);

			mRasterizerState = DX11Factory::CreateRasterizerState_BackfaceCulling(*mDevice.Get());
		}

		mDepthStencilViewHandle = mDepthStencilViewManager.lock()->Create(mWindowView.GetClientSize());

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

	void DX11Window::OnResize()
	{
		const Vector2ui windowSize = mWindowView.GetClientSize();

		if (windowSize.x == 0 || windowSize.y == 0)
		{
			return;
		}

		mContext->OMSetRenderTargets(0, nullptr, nullptr);

		// Release old references safely
		mBackBuffer.Reset();
		mDepthStencilViewManager.lock()->Get(mDepthStencilViewHandle).Reset();

		// Resize swap chain buffers
		HRESULT hr = mSwapChain->ResizeBuffers(0, windowSize.x, windowSize.y, DXGI_FORMAT_UNKNOWN, 0);
		WIN_CHECK_HRESULT(hr);

		// Create RTV
		auto backBuffer = DX11Factory::GetBackBuffer(*mSwapChain.Get());
		mBackBuffer.InitRenderTargetView(*backBuffer.Get(), windowSize);

		mDepthStencilViewManager.lock()->Initialize(mDepthStencilViewHandle, windowSize);
	}

	void DX11Window::BindBackBuffer()
	{
		mBackBuffer.Set(mDepthStencilViewManager.lock()->Get(mDepthStencilViewHandle).Get());

		auto viewport = DX11Factory::CreateViewport(mWindowView.GetClientSize());

		mContext->RSSetViewports(1, &viewport);
	}
}
