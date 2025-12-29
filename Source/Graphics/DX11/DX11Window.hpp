#pragma once
#include <unordered_map>
#include <memory>
#include <array>
#include <string>
#include <vector>

#include "Graphics/GraphicsDeclarations.hpp"
#include "Graphics/DX11/Renderer/DX11Renderer.hpp"
#include "Graphics/GraphicsSettings.hpp"
#include "Graphics/DX11/RenderTarget/DX11RenderTarget.hpp"
#include "Graphics/DX11/DX11SamplerState.hpp"
#include "Graphics/DX11/ImGui/DX11ImGuiWindow.hpp"
#include "Graphics/DX11/RenderTarget/DX11RenderTargetManager.hpp"
#include "Graphics/DX11/DX11DepthStencilViewManager.hpp"
#include "Utility/Asset/AssetLoader.hpp"
#include "Utility/SystemTimer.hpp"
#include "Engine/OperatingSystem/WindowView.hpp"


#ifdef _WIN32

#include <wrl/client.h>
#include <d3d11.h>

namespace Simple
{
	class Win_Window;
}

namespace Simple
{
	class DX11Window final
	{
	public:

		DX11Window(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,
			WindowView windowView, std::shared_ptr<AssetManager> assetManager, std::shared_ptr<GraphicsSettings> graphicsSettings, 
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