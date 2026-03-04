#pragma once

#ifdef _WIN32

#include "Engine/Win/WinDefines.hpp"

#include "Engine/Graphics/DX11/ConstantBuffer/DX11ConstantBuffer.hpp"
#include "Engine/Utility/Vertex.hpp"
#include "Engine/Graphics/ConstantBufferData.hpp"
#include "Engine/Graphics/DX11/DX11RenderFunctions.hpp"
#include "Engine/Utility/MeshData.hpp"

#include <wrl/client.h>
#include <d3d11.h>

namespace Simple
{

	class DX11MeshRenderer final
	{
	public:

		DX11MeshRenderer() = default;

		void Init(ID3D11Device& device, MeshData<Vertex>&& meshData);

		template<typename GetTransformMatrixFunc>
		void Render(ID3D11DeviceContext& context, PixelShaderAssetHandle pixelShader, VertexShaderAssetHandle vertexShader, const auto& drawList, GetTransformMatrixFunc&& getTransformMatrixFunction)
		{
			RenderMeshes(context, drawList, mMeshData, mTransformBuffer, mVertexBuffer.Get(), mIndexBuffer.Get(), pixelShader, vertexShader, std::forward<GetTransformMatrixFunc>(getTransformMatrixFunction));
		}

		template<typename GetTransformMatrixFunc>
		void Render(ID3D11DeviceContext& context, PixelShaderAssetHandle pixelShader, VertexShaderAssetHandle vertexShader, const auto& drawList, DX11ConstantBuffer<ColorBufferData>& colorCB, GetTransformMatrixFunc&& getTransformMatrixFunction)
		{
			PROFILER_FUNCTION();
			RenderMeshes(context, drawList, mMeshData, mTransformBuffer, colorCB, mVertexBuffer.Get(), mIndexBuffer.Get(), pixelShader, vertexShader, std::forward<GetTransformMatrixFunc>(getTransformMatrixFunction));
		}

	private:

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;

		DX11ConstantBuffer<TransformBufferData> mTransformBuffer;

		MeshData<Vertex> mMeshData;
	};
}

#endif