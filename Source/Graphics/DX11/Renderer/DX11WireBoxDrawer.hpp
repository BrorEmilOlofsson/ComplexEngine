#pragma once
#include "Utility/Math/Matrix4x4.hpp"
#include "Graphics/DX11/Mesh/DX11Mesh.hpp"
#include "Utility/Color.hpp"
#include "Utility/Shapes/AABB3.hpp"
#include "Utility/Asset/VertexShaderAsset.hpp"
#include "Utility/Asset/PixelShaderAsset.hpp"
#include "Graphics/ConstantBufferData.hpp"
#include "Graphics/DX11/ConstantBuffer/DX11ConstantBuffer.hpp"
#include "Utility/MeshData.hpp"

#ifdef _WIN32

namespace Simple
{
	class AssetManager;
}

namespace Simple
{

	class DX11WireBoxDrawer final
	{
	public:

		DX11WireBoxDrawer() = default;

		void Init(ID3D11Device& device, AssetManager& assetManager);
		void Render(const AABB3f& aabb, const Matrix4x4f& modelToWorldMatrix, const Color& color, ID3D11DeviceContext& context);

	private:

		void InitMeshData2D(AssetManager& assetManager);
		void InitMeshData3D(ID3D11Device& device, AssetManager& assetManager);

	private:

		MeshData<Vertex> mMeshData3D;
		MeshData<Vertex> mMeshData2D;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;

		PixelShaderAssetHandle mPixelShader3D;
		VertexShaderAssetHandle mVertexShader3D;
		PixelShaderAssetHandle mPixelShader2D;
		VertexShaderAssetHandle mVertexShader2D;

		DX11ConstantBuffer<TransformBufferData> mTransformBuffer;
	};
}

#endif