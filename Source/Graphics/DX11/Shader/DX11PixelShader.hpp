#pragma once
#include <filesystem>

#ifdef _WIN32

#include <d3d11.h>
#include <wrl/client.h>

struct ID3D11PixelShader;
struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Simple
{
	
	class DX11PixelShader final
	{
	public:

		DX11PixelShader(
			Microsoft::WRL::ComPtr<ID3D11Device> device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, 
			Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader, const std::filesystem::path& path);

		void Bind();

		[[nodiscard]] const std::filesystem::path& GetPath() const
		{
			return mPath;
		}

	private:

		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		std::filesystem::path mPath;

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
	};
}

#endif