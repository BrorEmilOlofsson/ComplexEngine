#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11Texture.hpp"
#include "Engine/Utility/Algorithm.hpp"
#include "Engine/Graphics/DX11/DX11Factory.hpp"
#include "Engine/Utility/Win/WinException.hpp"
#include <d3d11.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <External/stb/stb_image.h>

namespace CLX
{
	DX11Texture::DX11Texture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext)
		: mShaderResourceView(srv)
		, mDeviceContext(deviceContext)
	{
	}

	void DX11Texture::Bind(const unsigned int slot)
	{
		mDeviceContext->PSSetShaderResources(slot, 1, mShaderResourceView.GetAddressOf());
	}

	void DX11Texture::Bind()
	{
		mDeviceContext->PSSetShaderResources(mSlot, 1, mShaderResourceView.GetAddressOf());
	}

	void DX11Texture::Unbind() const
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		mDeviceContext->PSSetShaderResources(mSlot, 1, &nullSRV);
	}

	void DX11Texture::SetSlot(const unsigned int slot)
	{
		mSlot = slot;
	}

	unsigned int DX11Texture::GetSlot() const
	{
		return mSlot;
	}

	ID3D11ShaderResourceView* DX11Texture::GetShaderResourceView()
	{
		return mShaderResourceView.Get();
	}
}