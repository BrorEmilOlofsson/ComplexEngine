#pragma once
#include "Graphics/Shader/PixelShader.hpp"
#include "Graphics/Shader/VertexShader.hpp"
#include "Utility/MeshData.hpp"
#include "Utility/Asset/PixelShaderAsset.hpp"
#include "Utility/Asset/VertexShaderAsset.hpp"
#include "Graphics/RenderTarget/RenderTargetView.hpp"
#include "Graphics/DX11/DX11SamplerState.hpp"

namespace Simple
{

	template<std::ranges::range R, typename GetTransformMatrixFunc>
	void RenderMeshes(ID3D11DeviceContext& context, const R& drawList, MeshData<Vertex>& meshData,
		DX11ConstantBuffer<TransformBufferData>& transformCB, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer,
		PixelShaderAssetHandle pixelShader, VertexShaderAssetHandle vertexShader, GetTransformMatrixFunc&& getTransformMatrixFunction)
	{
		if (drawList.empty())
		{
			return;
		}
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		pixelShader->Bind();
		vertexShader->Bind();

		context.IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context.IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		transformCB.Bind(context);

		for (const auto& mesh : drawList)
		{

			TransformBufferData objectBuffer
			{
				.modelWorldMatrix = getTransformMatrixFunction(mesh)
			};

			transformCB.Update(objectBuffer, context);

			context.DrawIndexed(static_cast<UINT>(meshData.indices.size()), 0, 0);
		}
	}

	template<std::ranges::range R, typename GetTransformMatrixFunc>
	void RenderMeshes(ID3D11DeviceContext& context, const R& drawList, const MeshData<Vertex>& meshData,
		DX11ConstantBuffer<TransformBufferData>& transformCB, DX11ConstantBuffer<ColorBufferData>& colorCB, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer,
		PixelShaderAssetHandle pixelShader, VertexShaderAssetHandle vertexShader, GetTransformMatrixFunc&& getTransformMatrixFunction)
	{
		if (drawList.empty())
		{
			return;
		}
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		pixelShader->Bind();
		vertexShader->Bind();

		context.IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context.IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		transformCB.Bind(context);
		colorCB.Bind(context);

		for (const auto& mesh : drawList)
		{
			colorCB.Update(ColorBufferData{ mesh.color }, context);

			TransformBufferData objectBuffer
			{
				.modelWorldMatrix = getTransformMatrixFunction(mesh)
			};

			transformCB.Update(objectBuffer, context);

			context.DrawIndexed(static_cast<UINT>(meshData.indices.size()), 0, 0);
		}
	}

	inline void RenderFullScreen(ID3D11DeviceContext& context, RenderTargetView renderTargetView, DX11SamplerState& samplerState,
		VertexShaderAssetHandle vertexShader, PixelShaderAssetHandle pixelShader)
	{
		ID3D11ShaderResourceView* srv = static_cast<ID3D11ShaderResourceView*>(renderTargetView.GetSRV());
		context.PSSetShaderResources(0, 1, &srv);

		// Bind sampler state (linear clamp for example)
		samplerState.Bind(context);

		// Bind fullscreen shaders
		pixelShader->Bind();
		vertexShader->Bind();

		// No vertex buffer needed: fullscreen triangle
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context.IASetInputLayout(nullptr);

		// Issue fullscreen draw
		context.Draw(3, 0);

	}
}