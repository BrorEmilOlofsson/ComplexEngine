#pragma once
#include "Graphics/DX11/RenderTarget/DX11RenderTarget.hpp"
#include "Utility/Math/Vector2.hpp"


#ifdef _WIN32

#include <wrl/client.h>
#include <d3d11.h>

namespace Simple
{

	class DX11GBuffer final
	{
	public:

		explicit DX11GBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Device> device, Vector2ui size);

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

		void Resize(Vector2ui size);

		[[nodiscard]] Vector2ui GetSize() const { return mSize; }
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

		Vector2ui mSize = Vector2ui::Zero();
	};
}

#endif