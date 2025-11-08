#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11GBuffer.hpp"
#include "Graphics/DX11/DX11Factory.hpp"
#include "Graphics/GraphicsConstants.hpp"

namespace Simple
{

	DX11GBuffer::DX11GBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Device> device, const Vector2ui size)
		: mAlbedoRT(context, device, *DX11Factory::CreateRenderTargetTexture(*device.Get(), DX11Factory::CreateRenderTargetTextureDesc(size)).Get(), size)
		, mNormalRT(context, device, *DX11Factory::CreateRenderTargetTexture(*device.Get(), DX11Factory::CreateRenderTargetTextureDesc(size)).Get(), size)
		, mMaterialRT(context, device, *DX11Factory::CreateRenderTargetTexture(*device.Get(), DX11Factory::CreateRenderTargetTextureDesc(size)).Get(), size)
		, mPositionRT(context, device, *DX11Factory::CreateRenderTargetTexture(*device.Get(), DX11Factory::CreateRenderTargetTextureDesc(size)).Get(), size)
		, mObjectIDTexture(DX11Factory::CreateRenderTargetTexture(*device.Get(), DX11Factory::CreateObjectSelectionTextureDesc(size)))
		, mObjectIDRT(context, device, *mObjectIDTexture.Get(), size)
		, mContext(context)
		, mDevice(device)
	{
		mDepthStencilView = DX11Factory::CreateDepthStencilView(*device.Get(), size);
		mSize = size;


		// During Initialization
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_R32_UINT;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

		mDevice->CreateUnorderedAccessView(mObjectIDTexture.Get(), &uavDesc, &mObjectIDUAV);
	}

	void DX11GBuffer::Clear()
	{
		static constexpr float clearAlbedo[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		mContext->ClearRenderTargetView(mAlbedoRT.GetRenderTargetView(), clearAlbedo);

		static constexpr float clearNormal[4] = { 0.5f, 0.5f, 1.0f, 0.0f }; // no metal/rough here
		mContext->ClearRenderTargetView(mNormalRT.GetRenderTargetView(), clearNormal);

		static constexpr float clearMaterial[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		mContext->ClearRenderTargetView(mMaterialRT.GetRenderTargetView(), clearMaterial);

		static constexpr float clearPos[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		mContext->ClearRenderTargetView(mPositionRT.GetRenderTargetView(), clearPos);

		static constexpr float c[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		mContext->ClearRenderTargetView(mObjectIDRT.GetRenderTargetView(), c);



		UINT clearValue[4] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
		mContext->ClearUnorderedAccessViewUint(mObjectIDUAV.Get(), clearValue);

		mContext->ClearDepthStencilView(mDepthStencilView.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f,   // Depth = max
			0);     // Stencil = 0
	}

	void DX11GBuffer::Resize(Vector2ui size)
	{
		mAlbedoRT.Resize(*DX11Factory::CreateRenderTargetTexture(
			*mDevice.Get(),
			DX11Factory::CreateRenderTargetTextureDesc(size)
		).Get(), size);
		mNormalRT.Resize(*DX11Factory::CreateRenderTargetTexture(
			*mDevice.Get(),
			DX11Factory::CreateRenderTargetTextureDesc(size)
		).Get(), size);
		mMaterialRT.Resize(*DX11Factory::CreateRenderTargetTexture(
			*mDevice.Get(),
			DX11Factory::CreateRenderTargetTextureDesc(size)
		).Get(), size);
		mPositionRT.Resize(*DX11Factory::CreateRenderTargetTexture(
			*mDevice.Get(),
			DX11Factory::CreateRenderTargetTextureDesc(size)
		).Get(), size);
		mObjectIDTexture = DX11Factory::CreateRenderTargetTexture(*mDevice.Get(), DX11Factory::CreateObjectSelectionTextureDesc(size));


		// During Initialization
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_R32_UINT;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

		mDevice->CreateUnorderedAccessView(mObjectIDTexture.Get(), &uavDesc, &mObjectIDUAV);

		mObjectIDRT.Resize(*mObjectIDTexture.Get(), size);
		mDepthStencilView = DX11Factory::CreateDepthStencilView(*mDevice.Get(), size);
		mSize = size;
	}

	void DX11GBuffer::SetRenderTargets()
	{
		auto rtvArray = GetRTVArray();
		mContext->OMSetRenderTargets(
			static_cast<UINT>(rtvArray.size()),
			rtvArray.data(),
			mDepthStencilView.Get()
		);
	}

	void DX11GBuffer::SetShaderResources()
	{
		auto srvArray = GetSRVArray();
		mContext->PSSetShaderResources(
			TextureSlots::GBufferStart, 
			static_cast<UINT>(srvArray.size()), 
			srvArray.data()
		);
	}
}