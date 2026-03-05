#pragma once


#ifdef _WIN32

#include "Engine/Win/WinDefines.hpp"

#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Graphics/DX11/Mesh/DX11Mesh.hpp"
#include "Engine/Utility/Color.hpp"
#include "Engine/Math/Shapes/AABB3.hpp"
#include "Engine/Asset/AssetTypes/VertexShaderAsset.hpp"
#include "Engine/Asset/AssetTypes/PixelShaderAsset.hpp"
#include "Engine/Asset/AssetManager.hpp"
#include "Engine/Graphics/ConstantBufferData.hpp"
#include "Engine/Graphics/DX11/ConstantBuffer/DX11ConstantBuffer.hpp"
#include "Engine/Utility/MeshData.hpp"

#include <wrl/client.h>
#include <d3d11.h>

namespace CLX
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