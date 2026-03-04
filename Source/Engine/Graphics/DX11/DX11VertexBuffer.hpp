#pragma once


#ifdef _WIN32

#include <d3d11.h>
#include <wrl/client.h>

namespace CLX
{


	class DX11VertexBuffer
	{
	public:

		DX11VertexBuffer(ID3D11Device& device, const void* data, unsigned int sizeInBytes);
		
		DX11VertexBuffer(const DX11VertexBuffer&) = delete;
		DX11VertexBuffer(DX11VertexBuffer&&) noexcept = default;
		DX11VertexBuffer& operator=(const DX11VertexBuffer&) = delete;
		DX11VertexBuffer& operator=(DX11VertexBuffer&&) noexcept = default;

		[[nodiscard]] Microsoft::WRL::ComPtr<ID3D11Buffer> Get()
		{
			return mVertexBuffer;
		}

	private:


		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;

	};

}

#endif