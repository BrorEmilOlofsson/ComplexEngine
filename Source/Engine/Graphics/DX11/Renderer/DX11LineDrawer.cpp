#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11LineDrawer.hpp"
#include "Engine/Asset/AssetManager.hpp"
#include "Engine/Graphics/Shader/PixelShader.hpp"
#include "Engine/Graphics/Shader/VertexShader.hpp"

namespace CLX
{

	

	void DX11LineDrawer::Init(ID3D11Device& device, AssetManager& assetManager)
	{
		mPixelShader = assetManager.GetPixelShader(GetPath(ePixelShaderType::Line));
		mVertexShader = assetManager.GetVertexShader(GetPath(eVertexShaderType::Default));

		CreateBuffers(device);
		CreateInstanceBuffer(device);
		mTransformBuffer.Init(device, ConstantBufferSlots::Transform);
	}

	void DX11LineDrawer::Render(const DrawLine line, ID3D11DeviceContext& context)
	{
		mData.meshData.vertices[0].position = line.startPosition;
		mData.meshData.vertices[0].w = 1.0f;

		mData.meshData.vertices[1].position = line.endPosition;
		mData.meshData.vertices[1].w = 1.0f;

		for (auto& vertex : mData.meshData.vertices)
		{
			vertex.color = line.color;
		}

		context.UpdateSubresource(mData.vertexBuffer.Get(), 0, nullptr, mData.meshData.vertices.data(), 0, 0);

		TransformBufferData objectBuffer = {};
		objectBuffer.modelWorldMatrix = Matrix4x4f::Identity();

		mTransformBuffer.Bind(mTransformBuffer.GetSlot(), context);
		mTransformBuffer.Update(objectBuffer, context);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		mPixelShader->Bind();
		mVertexShader->Bind();

		context.IASetVertexBuffers(0, 1, mData.vertexBuffer.GetAddressOf(), &stride, &offset);
		context.IASetIndexBuffer(mData.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		context.DrawIndexed(static_cast<UINT>(mData.meshData.indices.size()), 0, 0);
	}

	void DX11LineDrawer::Render(const std::span<const DrawLine> lines, ID3D11DeviceContext& context)
	{
		//mInstanceData.meshData.vertices.resize(InstanceSizeLimit * 2, Vertex());
		for (size_t i = 0; i < lines.size() / InstanceSizeLimit + 1; i++)
		{
			std::size_t start = i * InstanceSizeLimit;
			std::size_t count = Min<size_t>(InstanceSizeLimit, lines.size() - start);
			RenderInstanced(lines.subspan(start, count), context);
		}
	}

	void DX11LineDrawer::RenderInstanced(const std::span<const DrawLine> lines, ID3D11DeviceContext& context)
	{
		const size_t vertexCount = lines.size() * 2;

		for (size_t i = 0; i < lines.size(); ++i)
		{
			const size_t startVertex = i * 2;
			const size_t endVertex = startVertex + 1;

			mInstanceData.meshData.vertices[startVertex].position = lines[i].startPosition;
			mInstanceData.meshData.vertices[startVertex].color = lines[i].color;

			mInstanceData.meshData.vertices[endVertex].position = lines[i].endPosition;
			mInstanceData.meshData.vertices[endVertex].color = lines[i].color;
		}

		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			mInstanceData.meshData.indices[i] = i;
		}

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		context.Map(mInstanceData.vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		std::memcpy(mappedResource.pData, mInstanceData.meshData.vertices.data(), sizeof(Vertex) * vertexCount);
		context.Unmap(mInstanceData.vertexBuffer.Get(), 0);

		context.Map(mInstanceData.indexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		std::memcpy(mappedResource.pData, mInstanceData.meshData.indices.data(), sizeof(UINT) * vertexCount);
		context.Unmap(mInstanceData.indexBuffer.Get(), 0);

		TransformBufferData objectBuffer
		{
			.modelWorldMatrix = Matrix4x4f::Identity()
		};

		mTransformBuffer.Bind(context);
		mTransformBuffer.Update(objectBuffer, context);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		mPixelShader->Bind();
		mVertexShader->Bind();

		context.IASetVertexBuffers(0, 1, mInstanceData.vertexBuffer.GetAddressOf(), &stride, &offset);
		context.IASetIndexBuffer(mInstanceData.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		context.DrawIndexed(static_cast<UINT>(vertexCount), 0, 0);
	}

	void DX11LineDrawer::CreateBuffers(ID3D11Device& device)
	{
		constexpr unsigned int numVertices = 2;
		mData.meshData.vertices.resize(numVertices);

		mData.meshData.vertices[0].position = Point3f();

		for (unsigned int i = 0; i < mData.meshData.vertices.size(); ++i)
		{
			mData.meshData.indices.push_back(i);
		}

		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * numVertices;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = mData.meshData.vertices.data();
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		HRESULT result = device.CreateBuffer(&vertexBufferDesc, &vertexData, &mData.vertexBuffer);

		if (FAILED(result))
		{
			assert(false && "failed to create VertexBuffer");
		}

		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned int) * numVertices;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = mData.meshData.indices.data();
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		result = device.CreateBuffer(&indexBufferDesc, &indexData, &mData.indexBuffer);

		if (FAILED(result))
		{
			assert(false && "failed to create IndexBuffer");
		}
	}

	void DX11LineDrawer::CreateInstanceBuffer(ID3D11Device& device)
	{
		mInstanceData.meshData.vertices.resize(InstanceSizeLimit * 2);
		mInstanceData.meshData.indices.resize(InstanceSizeLimit * 2);

		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * InstanceSizeLimit * 2;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = mInstanceData.meshData.vertices.data();
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		HRESULT result = device.CreateBuffer(&vertexBufferDesc, &vertexData, &mInstanceData.vertexBuffer);

		if (FAILED(result))
		{
			assert(false && "failed to create VertexBuffer");
		}

		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		indexBufferDesc.ByteWidth = sizeof(unsigned int) * InstanceSizeLimit * 2;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = mInstanceData.meshData.indices.data();
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		result = device.CreateBuffer(&indexBufferDesc, &indexData, &mInstanceData.indexBuffer);

		if (FAILED(result))
		{
			assert(false && "failed to create IndexBuffer");
		}
	}
}