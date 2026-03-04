#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11MeshRenderer.hpp"

namespace CLX
{

	void DX11MeshRenderer::Init(ID3D11Device& device, MeshData<Vertex>&& meshData)
	{
		mMeshData = std::move(meshData);
		mTransformBuffer.Init(device, ConstantBufferSlots::Transform);

		mVertexBuffer = DX11Factory::CreateVertexBuffer(device, mMeshData.vertices.data(), static_cast<UINT>(sizeof(decltype(mMeshData.vertices)::value_type) * mMeshData.vertices.size()));
		mIndexBuffer = DX11Factory::CreateIndexBuffer(device, mMeshData.indices.data(), static_cast<UINT>(sizeof(decltype(mMeshData.indices)::value_type) * mMeshData.indices.size()));
	}
}