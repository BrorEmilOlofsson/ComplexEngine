#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11RenderTarget.hpp"
#include "Utility/Win/WinException.hpp"
#include "Graphics/DX11/DX11Factory.hpp"

namespace Simple
{

	DX11RenderTarget::DX11RenderTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Device> device)
		: mContext(std::move(context))
		, mDevice(std::move(device))
	{
	}

	void DX11RenderTarget::Init(ID3D11Resource& resource, Vector2ui size, std::optional<D3D11_RENDER_TARGET_VIEW_DESC> desc)
	{
		const HRESULT result = mDevice->CreateShaderResourceView(&resource, nullptr, mShaderResourceView.GetAddressOf());
		
		WIN_CHECK_HRESULT(result);

		InitRenderTargetView(resource, size, desc);
	}

	void DX11RenderTarget::InitRenderTargetView(ID3D11Resource& resource, Vector2ui size, std::optional<D3D11_RENDER_TARGET_VIEW_DESC> descOpt)
	{
		mSize = size;
		if (descOpt)
		{
			mDesc = descOpt.value();
		}
		const D3D11_RENDER_TARGET_VIEW_DESC* desc = descOpt.has_value() ? &descOpt.value() : nullptr;
		
		const HRESULT result = mDevice->CreateRenderTargetView(&resource, desc, mRenderTargetView.GetAddressOf());
		
		WIN_CHECK_HRESULT(result);
	}

	void DX11RenderTarget::Set(ID3D11DepthStencilView& depthStencilView)
	{
		mContext->OMSetRenderTargets(1u, mRenderTargetView.GetAddressOf(), &depthStencilView);
	}

	void DX11RenderTarget::Clear(Color color)
	{
		mContext->ClearRenderTargetView(mRenderTargetView.Get(), &color.r);
	}

	void DX11RenderTarget::Resize(Vector2ui size)
	{
		auto renderTargetTexture = DX11Factory::CreateRenderTargetTexture(
			*mDevice.Get(), 
			DX11Factory::CreateRenderTargetTextureDesc(size)
		);
		Init(*renderTargetTexture.Get(), size, mDesc);
	}
}