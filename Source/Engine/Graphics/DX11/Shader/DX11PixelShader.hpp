#pragma once

#ifdef _WIN32

#include <d3d11.h>
#include <wrl/client.h>

namespace CLX
{
	
	class DX11PixelShader final
	{
	public:

		DX11PixelShader(
			Microsoft::WRL::ComPtr<ID3D11Device> device,
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, 
			Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader);

		void Bind();

	private:

		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
	};
}

#endif