#include "Engine/Precompiled/EnginePch.hpp"
#include "Engine/Graphics/DX11/Mesh/DX11Mesh.hpp"
#include "Engine/Graphics/DX11/DX11Factory.hpp"

#undef min
#undef max

namespace Simple
{

	DX11Mesh::DX11Mesh(const MeshData<Vertex>& meshData, const std::filesystem::path& meshName, const std::filesystem::path& relativePath, 
		ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext)
		: mDeviceContext(deviceContext)
		, mMeshData(meshData)
		, mVertexBuffer(device, mMeshData.vertices.data(), static_cast<unsigned int>(sizeof(Vertex) * mMeshData.vertices.size()))
	{
        auto positionRange = mMeshData.vertices | std::views::transform([](const Vertex& vertex) { return vertex.position; });
		mBoundingBox = CreateAABB3FromPoints(positionRange);
		mIndexBuffer = DX11Factory::CreateIndexBuffer(device, mMeshData.indices.data(), sizeof(unsigned int) * static_cast<unsigned int>(mMeshData.indices.size()));

		mName = meshName;
		mRelativePath = std::filesystem::relative(relativePath);
	}

	const AABB3f& DX11Mesh::GetBoundingBox() const
	{
		return mBoundingBox;
	}

	const std::filesystem::path& DX11Mesh::GetName() const
	{
		return mName;
	}

	const std::filesystem::path& DX11Mesh::GetPath() const
	{
		return mRelativePath;
	}

	static void RenderMesh(ID3D11DeviceContext& context, DX11VertexBuffer& vertexBuffer,
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer, const MeshData<Vertex>& meshData)
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		context.IASetVertexBuffers(0, 1, vertexBuffer.Get().GetAddressOf(), &stride, &offset);
		context.IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context.DrawIndexed(static_cast<UINT>(meshData.indices.size()), 0, 0);
	}

	void DX11Mesh::Render()
	{
		RenderMesh(*mDeviceContext.Get(), mVertexBuffer, mIndexBuffer, mMeshData);
	}
}