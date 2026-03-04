#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11PixelShader.hpp"

#ifdef _WIN32

namespace CLX
{

	DX11PixelShader::DX11PixelShader(
		Microsoft::WRL::ComPtr<ID3D11Device> device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader, const std::filesystem::path& path)
		: mDevice(device)
		, mContext(context)
		, mPixelShader(pixelShader)
		, mPath(path)
	{
	}

	void DX11PixelShader::Bind()
	{
		mContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
	}
}

#endif