#pragma once
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
#include <unordered_map>
#include <memory>
#include <array>
#include <string>
#include <vector>

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
			Win_Window* window, std::shared_ptr<AssetManager> assetManager, std::shared_ptr<GraphicsSettings> graphicsSettings, 
			std::weak_ptr<DX11DepthStencilViewManager> dsvManager, bool instantiateImGui);

		void Init();

		void BeginFrame();
		void EndFrame(RenderContext* renderContext);
		void PrepareFrame();

		void OnWindowResize();

		void BindBackBuffer();


	private:

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
		Win_Window* mWindow = nullptr;
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