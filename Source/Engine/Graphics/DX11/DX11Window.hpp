#pragma once
#include <memory>


#ifdef _WIN32

#include "Engine/Win/WinDefines.hpp"

#include "Engine/Graphics/DX11/Renderer/DX11Renderer.hpp"
#include "Engine/Graphics/GraphicsSettings.hpp"
#include "Engine/Graphics/DX11/RenderTarget/DX11RenderTarget.hpp"
#include "Engine/Graphics/DX11/DX11SamplerState.hpp"
#include "Engine/Graphics/DX11/ImGui/DX11ImGuiWindow.hpp"
#include "Engine/Graphics/DX11/DX11DepthStencilViewManager.hpp"
#include "Engine/OperatingSystem/WindowView.hpp"

#include <wrl/client.h>
#include <d3d11.h>

namespace CLX
{
	class Win_Window;
}

namespace CLX
{
	class DX11Window final
	{
	public:

		DX11Window(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,
			WindowView windowView, std::weak_ptr<AssetManager> assetManager, std::weak_ptr<GraphicsSettings> graphicsSettings,
			std::weak_ptr<DX11DepthStencilViewManager> dsvManager, std::weak_ptr<DX11SamplerState> samplerState, bool instantiateImGui);

        DX11Window(const DX11Window&) = delete;
        DX11Window& operator=(const DX11Window&) = delete;
        DX11Window(DX11Window&&) = default;
        DX11Window& operator=(DX11Window&&) = default;

		void Init();

		void BeginFrame();
		void EndFrame(RenderContext* renderContext);
		void PrepareFrame();

		void OnResize();

		void BindBackBuffer();


	private:

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
		WindowView mWindowView;
		DX11RenderTarget mBackBuffer;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;
		DepthStencilViewHandle mDepthStencilViewHandle;

		std::weak_ptr<GraphicsSettings> mGraphicsSettings;
		std::weak_ptr<AssetManager> mAssetManager;
		std::weak_ptr<DX11DepthStencilViewManager> mDepthStencilViewManager;
		std::weak_ptr<DX11SamplerState> mSamplerState;
		std::optional<DX11ImGuiWindow> mImGuiWindow;

	};
}

#endif