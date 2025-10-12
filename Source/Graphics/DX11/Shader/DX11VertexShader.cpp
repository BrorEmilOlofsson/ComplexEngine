#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11VertexShader.hpp"
#include "Graphics/DX11/DX11Factory.hpp"

namespace Simple
{

	DX11VertexShader::DX11VertexShader(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader, Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout)
		: mDevice(device)
		, mContext(context)
		, mVertexShader(vertexShader)
		, mInputLayout(inputLayout)
	{
	}

	void DX11VertexShader::Bind()
	{
		mContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		mContext->IASetInputLayout(mInputLayout.Get());
	}
}
