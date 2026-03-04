#pragma once


#ifdef _WIN32

#include <span>

#include "Engine/Win/WinDefines.hpp"

#include "Engine/Graphics/DX11/Mesh/DX11Mesh.hpp"
#include "Engine/Utility/Color.hpp"
#include "Engine/Graphics/DX11/ConstantBuffer/DX11ConstantBuffer.hpp"
#include "Engine/Graphics/ConstantBufferData.hpp"
#include "Engine/Asset/PixelShaderAsset.hpp"
#include "Engine/Asset/VertexShaderAsset.hpp"
#include "Engine/Utility/DrawPrimitives/DrawPrimitives.hpp"

#include <d3d11.h>

namespace Simple
{
	class AssetManager;
}

namespace Simple
{
	class DX11LineDrawer final
	{
		struct Data
		{
			MeshData<Vertex> meshData;
			Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
			Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
		};

	public:

		DX11LineDrawer() = default;

		void Init(ID3D11Device& device, AssetManager& assetManager);

		void Render(const DrawLine line, ID3D11DeviceContext& context);
		void Render(const std::span<const DrawLine> lines, ID3D11DeviceContext& context);
		void RenderInstanced(const std::span<const DrawLine> lines, ID3D11DeviceContext& context);

	public:

		constexpr std::size_t GetInstanceSizeLimit() const
		{
			return InstanceSizeLimit;
		}

	private:

		void CreateBuffers(ID3D11Device& device);
		void CreateInstanceBuffer(ID3D11Device& device);

	private:

		Data mData;
		Data mInstanceData;

		DX11ConstantBuffer<TransformBufferData> mTransformBuffer;
		PixelShaderAssetHandle mPixelShader;
		VertexShaderAssetHandle mVertexShader;

		static constexpr size_t InstanceSizeLimit = 4046;
	};
}

#endif