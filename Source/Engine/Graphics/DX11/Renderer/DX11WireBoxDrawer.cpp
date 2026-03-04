#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11WireBoxDrawer.hpp"
#include "Engine/Asset/AssetManager.hpp"
#include "Engine/Graphics/Shader/PixelShader.hpp"
#include "Engine/Graphics/Shader/VertexShader.hpp"

#ifdef _WIN32

namespace Simple
{

	void DX11WireBoxDrawer::Init(ID3D11Device& device, AssetManager& assetManager)
	{
		InitMeshData2D(assetManager);
		InitMeshData3D(device, assetManager);
		mTransformBuffer.Init(device, ConstantBufferSlots::Transform);
	}

	void DX11WireBoxDrawer::Render(const AABB3f& aabb, const Matrix4x4f& modelToWorldMatrix, const Color& color, ID3D11DeviceContext& context)
	{
		PROFILER_FUNCTION();
		const Point3f minPoint = aabb.GetMin();
		const Point3f maxPoint = aabb.GetMax();

		Vertex down_SouthWest;
		down_SouthWest.position = { minPoint.x, minPoint.y, minPoint.z };
		Vertex down_SouthEast;
		down_SouthEast.position = { maxPoint.x, minPoint.y, minPoint.z };
		Vertex up_SouthWest;
		up_SouthWest.position = { minPoint.x, maxPoint.y, minPoint.z };
		Vertex up_SouthEast;
		up_SouthEast.position = { maxPoint.x, maxPoint.y, minPoint.z };
		Vertex down_NorthWest;
		down_NorthWest.position = { minPoint.x, minPoint.y, maxPoint.z };
		Vertex down_NorthEast;
		down_NorthEast.position = { maxPoint.x, minPoint.y, maxPoint.z };
		Vertex up_NorthWest;
		up_NorthWest.position = { minPoint.x, maxPoint.y, maxPoint.z };
		Vertex up_NorthEast;
		up_NorthEast.position = { maxPoint.x, maxPoint.y, maxPoint.z };

		mMeshData3D.vertices[0] = down_SouthWest;
		mMeshData3D.vertices[1] = down_SouthEast;
		mMeshData3D.vertices[2] = down_SouthEast;
		mMeshData3D.vertices[3] = up_SouthEast;
		mMeshData3D.vertices[4] = up_SouthEast;
		mMeshData3D.vertices[5] = up_SouthWest;
		mMeshData3D.vertices[6] = up_SouthWest;
		mMeshData3D.vertices[7] = down_SouthWest;

		mMeshData3D.vertices[8] = down_NorthWest;
		mMeshData3D.vertices[9] = down_NorthEast;
		mMeshData3D.vertices[10] = down_NorthEast;
		mMeshData3D.vertices[11] = up_NorthEast;
		mMeshData3D.vertices[12] = up_NorthEast;
		mMeshData3D.vertices[13] = up_NorthWest;
		mMeshData3D.vertices[14] = up_NorthWest;
		mMeshData3D.vertices[15] = down_NorthWest;

		mMeshData3D.vertices[16] = down_SouthWest;
		mMeshData3D.vertices[17] = down_NorthWest;
		mMeshData3D.vertices[18] = down_SouthEast;
		mMeshData3D.vertices[19] = down_NorthEast;
		mMeshData3D.vertices[20] = up_SouthWest;
		mMeshData3D.vertices[21] = up_NorthWest;
		mMeshData3D.vertices[22] = up_SouthEast;
		mMeshData3D.vertices[23] = up_NorthEast;

		for (auto& vertice : mMeshData3D.vertices)
		{
			vertice.color = color;
		}

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		context.Map(mVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, mMeshData3D.vertices.data(), sizeof(Vertex) * mMeshData3D.vertices.size());
		context.Unmap(mVertexBuffer.Get(), 0);

		//context->UpdateSubresource(myVertexBuffer.Get(), 0, nullptr, myMeshData3D.vertices.data(), 0, 0);

		TransformBufferData objectBuffer = {};
		objectBuffer.modelWorldMatrix = modelToWorldMatrix;

		mTransformBuffer.Bind(context);
		mTransformBuffer.Update(objectBuffer, context);
		mPixelShader3D->Bind();
		mVertexShader3D->Bind();

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		context.IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
		context.IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		context.DrawIndexed(static_cast<UINT>(mMeshData3D.vertices.size()), 0, 0);
	}

	void DX11WireBoxDrawer::InitMeshData2D(AssetManager& assetManager)
	{
		mPixelShader2D = assetManager.GetPixelShader("Shaders\\LinePS.cso");
		mVertexShader2D = assetManager.GetVertexShader("Shaders\\Line2DVS.cso");
	}

	void DX11WireBoxDrawer::InitMeshData3D(ID3D11Device& device, AssetManager& assetManager)
	{
		mPixelShader3D = assetManager.GetPixelShader("Shaders/LinePS.cso");
		mVertexShader3D = assetManager.GetVertexShader("Shaders/DefaultVS.cso");

		mMeshData3D.vertices.reserve(24);
		mMeshData3D.vertices.resize(24, Vertex{});

		for (unsigned int i = 0; i < mMeshData3D.vertices.size(); ++i)
		{
			mMeshData3D.indices.push_back(i);
		}

		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * static_cast<int>(mMeshData3D.vertices.size());
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = &mMeshData3D.vertices[0];
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		HRESULT result = device.CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);

		if (FAILED(result))
			assert(false && "failed to create VertexBuffer");

		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		indexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<int>(mMeshData3D.indices.size());
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = &mMeshData3D.indices[0];
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		result = device.CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);

		if (FAILED(result))
			assert(false && "failed to create IndexBuffer");
	}
}

#endif