#pragma once
#include <wrl/client.h>
#include <filesystem>

struct ID3D11VertexShader;
struct ID3D11InputLayout;
struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Simple
{

	class DX11VertexShader final
	{
	public:

		DX11VertexShader(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, 
			Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader, Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout);


		void Bind();

		const std::filesystem::path& GetPath() const
		{
			return mPath;
		}

	private:

		std::filesystem::path mPath;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
	};
}
