#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11VertexBuffer.hpp"
#include "Engine/Graphics/DX11/DX11Factory.hpp"

namespace CLX
{

	DX11VertexBuffer::DX11VertexBuffer(ID3D11Device& device, const void* data, unsigned int sizeInBytes)
		: mVertexBuffer(DX11Factory::CreateVertexBuffer(device, data, sizeInBytes))
	{
	}
}