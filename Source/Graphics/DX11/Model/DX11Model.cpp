#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11Model.hpp"

#undef min
#undef max

namespace Simple
{

	DX11Model::DX11Model(std::vector<DX11Mesh>&& meshData, const std::string& name, const std::filesystem::path& path,
		ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
		: mContext(context)
		, mName(name)
		, mPath(path)
		, mMeshes(std::move(meshData))
	{
		device;
		mBoundingBox = CreateBoundingBox(std::views::transform(mMeshes, [](const DX11Mesh& mesh) { return mesh.GetMeshData(); }));
	}

	const AABB3f& DX11Model::GetBoundingBox() const
	{
		return mBoundingBox;
	}

	const std::string& DX11Model::GetName() const
	{
		return mName;
	}

	const std::filesystem::path& DX11Model::GetPath() const
	{
		return mPath;
	}

	static void RenderMesh(ID3D11DeviceContext& context, Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer,
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer, const MeshData<Vertex>& meshData)
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		context.IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		context.IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context.DrawIndexed(static_cast<UINT>(meshData.indices.size()), 0, 0);
	}

	void DX11Model::Render()
	{
		for (auto& mesh : mMeshes)
		{
			mesh.Render();
		}
	}
}