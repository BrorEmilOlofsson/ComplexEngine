#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11GBuffer.hpp"
#include "Graphics/DX11/DX11Factory.hpp"

namespace Simple
{

	DX11GBuffer::DX11GBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Device> device)
		: mAlbedoRT(context, device)
		, mNormalRT(context, device)
		, mMaterialRT(context, device)
		, mPositionRT(context, device)
		, mContext(context)
		, mDevice(device)
	{
	}

	void DX11GBuffer::Init(Vector2ui size)
	{
		auto& device = *mDevice.Get();
		auto desc = DX11Factory::CreateRenderTargetTextureDesc(size);
		mAlbedoRT.Init(*DX11Factory::CreateRenderTargetTexture(device, desc).Get(), size);
		mNormalRT.Init(*DX11Factory::CreateRenderTargetTexture(device, desc).Get(), size);
		mMaterialRT.Init(*DX11Factory::CreateRenderTargetTexture(device, desc).Get(), size);
		mPositionRT.Init(*DX11Factory::CreateRenderTargetTexture(device, desc).Get(), size);
		mDepthStencilView = DX11Factory::CreateDepthStencilView(device, size);
		mSize = size;
	}

	void DX11GBuffer::Clear()
	{
		static constexpr float clearAlbedo[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		mContext->ClearRenderTargetView(mAlbedoRT.GetRenderTargetView(), clearAlbedo);

		static const float clearNormal[4] = { 0.5f, 0.5f, 1.0f, 0.0f }; // no metal/rough here
		mContext->ClearRenderTargetView(mNormalRT.GetRenderTargetView(), clearNormal);

		static const float clearMaterial[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		mContext->ClearRenderTargetView(mMaterialRT.GetRenderTargetView(), clearMaterial);

		static const float clearPos[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		mContext->ClearRenderTargetView(mPositionRT.GetRenderTargetView(), clearPos);

		mContext->ClearDepthStencilView(mDepthStencilView.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f,   // Depth = max
			0);     // Stencil = 0
	}

	void DX11GBuffer::Resize(Vector2ui size)
	{
		if (mSize == size)
		{
			return;
		}
		mAlbedoRT.Resize(size);
		mNormalRT.Resize(size);
		mMaterialRT.Resize(size);
		mPositionRT.Resize(size);
		mDepthStencilView = DX11Factory::CreateDepthStencilView(*mDevice.Get(), size);
		mSize = size;
	}
}