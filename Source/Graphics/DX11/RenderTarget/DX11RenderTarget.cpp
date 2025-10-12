#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11RenderTarget.hpp"
#include "Utility/Win/WinException.hpp"

namespace Simple
{

	DX11RenderTarget::DX11RenderTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
		: mContext(std::move(context))
	{
	}

	void DX11RenderTarget::Init(ID3D11Device& device, ID3D11Resource& resource, Vector2ui size, std::optional<D3D11_RENDER_TARGET_VIEW_DESC> desc)
	{

		/*D3D11_SHADER_RESOURCE_VIEW_DESC desc1{};
		desc1.*/

		const HRESULT result = device.CreateShaderResourceView(&resource, nullptr, mShaderResourceView.GetAddressOf());
		
		WIN_CHECK_HRESULT(result);

		InitRenderTargetView(device, resource, size, desc);

	}

	void DX11RenderTarget::InitRenderTargetView(ID3D11Device& device, ID3D11Resource& resource, Vector2ui size, std::optional<D3D11_RENDER_TARGET_VIEW_DESC> descOpt)
	{
		mSize = size;
		const D3D11_RENDER_TARGET_VIEW_DESC* desc = descOpt.has_value() ? &descOpt.value() : nullptr;

		const HRESULT result = device.CreateRenderTargetView(&resource, desc, mRenderTargetView.GetAddressOf());
		
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
}