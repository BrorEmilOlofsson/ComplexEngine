#pragma once
#include "Engine/Graphics/DX11/RenderTarget/DX11RenderTarget.hpp"
#include "Engine/Math/Vector2.hpp"


#ifdef _WIN32

#include <wrl/client.h>
#include <d3d11.h>

namespace CLX
{

	class DX11GBuffer final
	{
	public:

		explicit DX11GBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Device> device, Dimension2u size);

		[[nodiscard]] std::array<ID3D11RenderTargetView*, 5> GetRTVArray()
		{
			return
			{
				mAlbedoRT.GetRenderTargetView(),
				mNormalRT.GetRenderTargetView(),
				mMaterialRT.GetRenderTargetView(),
				mPositionRT.GetRenderTargetView(),
				mObjectIDRT.GetRenderTargetView()
			};
		}

		[[nodiscard]] std::array<ID3D11ShaderResourceView*, 5> GetSRVArray()
		{
			return
			{
				mAlbedoRT.GetShaderResourceView(),
				mNormalRT.GetShaderResourceView(),
				mMaterialRT.GetShaderResourceView(),
				mPositionRT.GetShaderResourceView(),
				mObjectIDRT.GetShaderResourceView()
			};
		}

		[[nodiscard]] ID3D11DepthStencilView* GetDepthStencilView()
		{
			return mDepthStencilView.Get();
		}

		void Clear();

		void Resize(Dimension2u size);

		[[nodiscard]] Dimension2u GetSize() const { return mSize; }

		void SetRenderTargets();
		void SetShaderResources();

	private:

		DX11RenderTarget mAlbedoRT;
		DX11RenderTarget mNormalRT;
		DX11RenderTarget mMaterialRT;
		DX11RenderTarget mPositionRT;
	public:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mObjectIDTexture;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> mObjectIDUAV;
	private:
		DX11RenderTarget mObjectIDRT;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;

		Dimension2u mSize = Dimension2u::Zero();
	};
}

#endif