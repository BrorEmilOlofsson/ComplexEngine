#pragma once

#ifdef _WIN32

#include <wrl/client.h>
#include <d3d11.h>
#include <filesystem>


namespace CLX
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

#endif // _WIN32