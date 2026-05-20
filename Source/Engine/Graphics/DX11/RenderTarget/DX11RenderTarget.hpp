#pragma once

#ifdef _WIN32

#include <d3d11.h>
#include <wrl/client.h>
#include <optional>
#include "Engine/Utility/Color.hpp"
#include "Engine/Math/Dimension2.hpp"

namespace CLX
{
	class DX11RenderTarget final
	{
	public:

		DX11RenderTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Device> device, 
			ID3D11Resource& resource, Dimension2u size, const bool createSRV = true, std::optional<D3D11_RENDER_TARGET_VIEW_DESC> rtvDesc = {}, std::optional<D3D11_SHADER_RESOURCE_VIEW_DESC> srvDesc = {});


		DX11RenderTarget(const DX11RenderTarget&) = delete;
		DX11RenderTarget(DX11RenderTarget&&) = default;
		DX11RenderTarget& operator=(const DX11RenderTarget&) = delete;
		DX11RenderTarget& operator=(DX11RenderTarget&&) = default;

		void InitShaderResourceView(ID3D11Resource& resource, std::optional<D3D11_SHADER_RESOURCE_VIEW_DESC> desc = {});
		void InitRenderTargetView(ID3D11Resource& resource, Dimension2u size, std::optional<D3D11_RENDER_TARGET_VIEW_DESC> desc = {});

		void Set();
		void Set(ID3D11DepthStencilView* depthStencilView);
		void Clear(Color color);

		void Resize(ID3D11Resource& resource, Dimension2u size);

		void Reset()
		{
			mRenderTargetView.Reset();
		}

		[[nodiscard]] ID3D11RenderTargetView* GetRenderTargetView();
		[[nodiscard]] ID3D11ShaderResourceView* GetShaderResourceView();
		[[nodiscard]] Dimension2u GetSize() const { return mSize; }

	private:

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Dimension2u mSize = Dimension2u::Zero();
		std::optional<D3D11_RENDER_TARGET_VIEW_DESC> mRTVDesc;
		std::optional<D3D11_SHADER_RESOURCE_VIEW_DESC> mSRVDesc;
	};

	inline ID3D11RenderTargetView* DX11RenderTarget::GetRenderTargetView()
	{
		return mRenderTargetView.Get();
	}

	inline ID3D11ShaderResourceView* DX11RenderTarget::GetShaderResourceView()
	{
		return mShaderResourceView.Get();
	}

	[[nodiscard]] inline void* GetRenderTargetSRV(DX11RenderTarget& renderTarget)
	{
		return renderTarget.GetShaderResourceView();
	}

}

#endif // _WIN32