#pragma once
#include <optional>
#include "Utility/Color.hpp"

#ifdef _WIN32

#include <d3d11.h>
#include <wrl/client.h>

namespace Simple
{
	class DX11RenderTarget final
	{
	public:

		DX11RenderTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

		void Init(ID3D11Device& device, ID3D11Resource& resource, Vector2ui size, std::optional<D3D11_RENDER_TARGET_VIEW_DESC> desc = {});
		void InitRenderTargetView(ID3D11Device& device, ID3D11Resource& resource, Vector2ui size, std::optional<D3D11_RENDER_TARGET_VIEW_DESC> desc = {});

		void Set(ID3D11DepthStencilView& depthStencilView);
		void Clear(Color color);

		[[nodiscard]] ID3D11RenderTargetView* GetRenderTargetView();
		[[nodiscard]] ID3D11ShaderResourceView* GetShaderResourceView();
		[[nodiscard]] Vector2ui GetSize() const { return mSize; }

	private:

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
		Vector2ui mSize = Vector2ui::Zero();

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