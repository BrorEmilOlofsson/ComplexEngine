#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11RenderTarget.hpp"
#include "Engine/Graphics/DX11/DX11Factory.hpp"

#ifdef _WIN32

namespace CLX
{

	DX11RenderTarget::DX11RenderTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Device> device,
		ID3D11Resource& resource, const Dimension2u size, const bool createSRV, std::optional<D3D11_RENDER_TARGET_VIEW_DESC> rtvDesc, std::optional<D3D11_SHADER_RESOURCE_VIEW_DESC> srvDesc)
		: mContext(std::move(context))
		, mDevice(std::move(device))
		, mSize(size)
		, mRTVDesc(rtvDesc)
	{
		if (createSRV)
		{
			InitShaderResourceView(resource, srvDesc);
		}
		InitRenderTargetView(resource, size, rtvDesc);
	}

	void DX11RenderTarget::InitShaderResourceView(ID3D11Resource& resource, std::optional<D3D11_SHADER_RESOURCE_VIEW_DESC> descOpt)
	{
		mSRVDesc = descOpt;
		mShaderResourceView = DX11Factory::CreateShaderResourceView(*mDevice.Get(), resource, descOpt);
	}

	void DX11RenderTarget::InitRenderTargetView(ID3D11Resource& resource, Dimension2u size, std::optional<D3D11_RENDER_TARGET_VIEW_DESC> descOpt)
	{
		mSize = size;
		if (descOpt)
		{
			mRTVDesc = descOpt.value();
		}
		mRenderTargetView = DX11Factory::CreateRenderTargetView(*mDevice.Get(), resource, descOpt);
	}

	void DX11RenderTarget::Set()
	{
		Set(nullptr);
	}

	void DX11RenderTarget::Set(ID3D11DepthStencilView* depthStencilView)
	{
		mContext->OMSetRenderTargets(1u, mRenderTargetView.GetAddressOf(), depthStencilView);
	}

	void DX11RenderTarget::Clear(Color color)
	{
		mContext->ClearRenderTargetView(mRenderTargetView.Get(), &color.r);
	}

	void DX11RenderTarget::Resize(ID3D11Resource& resource, Dimension2u size)
	{
		if (mShaderResourceView)
		{
			InitShaderResourceView(resource, mSRVDesc);
		}
		InitRenderTargetView(resource, size, mRTVDesc);
	}
}

#endif