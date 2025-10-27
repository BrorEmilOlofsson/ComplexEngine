#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11Texture.hpp"
#include "Utility/Algorithm.hpp"
#include "Graphics/DX11/DX11Factory.hpp"
#include "Utility/Win/WinException.hpp"
#include <d3d11.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <External/stb/stb_image.h>

namespace Simple
{
	DX11Texture::DX11Texture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, const std::filesystem::path& relativePath)
		: mShaderResourceView(srv)
		, mDeviceContext(deviceContext)
		, mRelativePath(relativePath)
		, mName(relativePath.stem().string())
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

	const std::string& DX11Texture::GetName() const
	{
		return mName;
	}

	const std::filesystem::path& DX11Texture::GetRelativePath() const
	{
		return mRelativePath;
	}

	ID3D11ShaderResourceView* DX11Texture::GetShaderResourceView()
	{
		return mShaderResourceView.Get();
	}
}