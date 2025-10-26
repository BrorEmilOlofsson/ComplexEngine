#pragma once
#include "Utility/Asset/AssetLoader.hpp"
#include "Utility/Asset/AssetManager.hpp"
#include "Graphics/DX11/RenderTarget/DX11RenderTargetManager.hpp"
#include "Graphics/DX11/ImGui/DX11ImGuiFoundation.hpp"
#include "Graphics/DX11/Renderer/DX11Renderer.hpp"
#include "Graphics/DX11/DX11SamplerState.hpp"
#include "Graphics/DX11/ConstantBuffer/DX11ConstantBufferManager.hpp"
#include "Graphics/DX11/DX11DepthStencilViewManager.hpp"
#include "Utility/SystemTimer.hpp"
#include "Utility/GraphicsBufferData.hpp"
#include "Graphics/GraphicsSettings.hpp"

#ifdef _WIN32

#include <d3d11.h>
#include <wrl/client.h>

namespace Simple
{

	class DX11Foundation final
	{
	public:

		DX11Foundation();

		DX11Foundation(const DX11Foundation&) = delete;
		DX11Foundation& operator=(const DX11Foundation&) = delete;

		DX11Foundation(DX11Foundation&&) = default;
		DX11Foundation& operator=(DX11Foundation&&) = default;

		Microsoft::WRL::ComPtr<ID3D11Device> GetDevice()
		{
			return mDeviceAndContext.first;
		}

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext()
		{
			return mDeviceAndContext.second;
		}

		void BeginFrame(const GraphicsBufferData& bufferData);
		void EndFrame();
		void Init();
		void Shutdown();

		void Render(RenderState& renderState);

		[[nodiscard]] void SetAssetLoaders(AssetLoader& assetLoader) const;

		[[nodiscard]] std::function<void(AssetManager&)> GetDefaultAssetLoader();

		void SetAssetManager(std::shared_ptr<AssetManager> assetManager)
		{
			mAssetManager = std::move(assetManager);
			SetAssetLoaders(mAssetManager->GetAssetLoader());
			mAssetManager->SetDefaultLoader(GetDefaultAssetLoader());
		}

		void SetGraphicsSettings(std::shared_ptr<GraphicsSettings> graphicsSettings)
		{
			mGraphicsSettings = std::move(graphicsSettings);
		}

		RenderTargetView CreateRenderTarget(const Vector2ui& size)
		{
			return mRenderTargetManager.Create(size);
		}

		DepthStencilViewHandle CreateDepthStencilView(const Vector2ui& size)
		{
			return mDepthStencilViewManager->Create(size);
		}

		[[nodiscard]] RenderContext CreateRenderContext(const Vector2ui& size);

		[[nodiscard]] std::shared_ptr<DX11DepthStencilViewManager> GetDepthStencilViewManager()
		{
			return mDepthStencilViewManager;
		}

		[[nodiscard]] std::shared_ptr<DX11SamplerState> GetSamplerState()
		{
			return mSamplerState;
		}

	private:

		std::pair<Microsoft::WRL::ComPtr<ID3D11Device>, Microsoft::WRL::ComPtr<ID3D11DeviceContext>> mDeviceAndContext;

		DX11ImGuiFoundation mImGui;
		DX11ConstantBufferManager mConstantBufferManager;
		DX11Renderer mRenderer;
		DX11RenderTargetManager mRenderTargetManager;
		std::shared_ptr<DX11DepthStencilViewManager> mDepthStencilViewManager;
		std::shared_ptr<DX11SamplerState> mSamplerState;
		std::shared_ptr<AssetManager> mAssetManager;
		std::shared_ptr<GraphicsSettings> mGraphicsSettings;

		SystemTimerf mFrameTimer;
		SystemTimerd mTotalTimer;
	};
}

#endif