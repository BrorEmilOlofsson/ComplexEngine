#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11VertexBuffer.hpp"
#include "Graphics/DX11/DX11Factory.hpp"

namespace Simple
{

	DX11VertexBuffer::DX11VertexBuffer(ID3D11Device& device, const void* data, unsigned int sizeInBytes)
		: mVertexBuffer(DX11Factory::CreateVertexBuffer(device, data, sizeInBytes))
	{
	}
}