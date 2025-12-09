#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11WireSphereDrawer.hpp"
#include "Graphics/Model/Factory/ShapeCreator3000.hpp"
#include "Graphics/DX11/ConstantBuffer/DX11ConstantBuffer.hpp"
#include "Utility/Asset/AssetManager.hpp"
#include "Graphics/Shader/PixelShader.hpp"
#include "Graphics/Shader/VertexShader.hpp"

#ifdef _WIN32

namespace Simple
{

	void DX11WireSphereDrawer::Init(ID3D11Device& device, AssetManager& assetManager)
	{
		mPixelShader = assetManager.GetPixelShader("Shaders/LinePS.cso");
		mVertexShader = assetManager.GetVertexShader("Shaders/DefaultVS.cso");

		MeshData<Vertex> sphereData = ShapeCreator3000::CreateSphere(1.0f, 10, 10);

		mMeshData.vertices = sphereData.vertices;
		mMeshData.indices = sphereData.indices;

		for (auto& vertex : mMeshData.vertices)
		{
			vertex.color = Colors::Yellow;
		}

		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * static_cast<int>(mMeshData.vertices.size());
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = mMeshData.vertices.data();
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		HRESULT result = device.CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);

		if (FAILED(result))
		{
			assert(false && "failed to create VertexBuffer");
		}

		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<int>(mMeshData.indices.size());
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = mMeshData.indices.data();
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		result = device.CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);

		if (FAILED(result))
		{
			assert(false && "failed to create IndexBuffer");
		}

		mTransformBuffer.Init(device, ConstantBufferSlots::Transform);
	}

	void DX11WireSphereDrawer::Render(const DrawSphere& drawSphere, ID3D11DeviceContext& context)
	{
		PROFILER_FUNCTION()
		for (auto& vertex : mMeshData.vertices)
		{
			vertex.color = drawSphere.color;
		}

		context.UpdateSubresource(mVertexBuffer.Get(), 0, nullptr, mMeshData.vertices.data(), 0, 0);

		Transform transform;
        const float radius = drawSphere.sphere.GetRadius().Value();
		transform.SetScale(Vector3f(radius, radius, radius));
		transform.SetPosition(drawSphere.sphere.GetCenter());

		TransformBufferData objectBuffer = {};
		objectBuffer.modelWorldMatrix = transform.GetMatrix();

		mTransformBuffer.Bind(mTransformBuffer.GetSlot(), context);
		mTransformBuffer.Update(objectBuffer, context);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		mPixelShader->Bind();
		mVertexShader->Bind();

		context.IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
		context.IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		context.DrawIndexed(static_cast<UINT>(mMeshData.indices.size()), 0, 0);
	}
}

#endif