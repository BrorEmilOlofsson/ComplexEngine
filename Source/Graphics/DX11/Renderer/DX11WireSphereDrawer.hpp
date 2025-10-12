#pragma once
#include "Utility/Color.hpp"
#include "Utility/Shapes/Sphere.hpp"
#include "Graphics/DX11/Mesh/DX11Mesh.hpp"
#include "Graphics/ConstantBufferData.hpp"
#include "Utility/DrawPrimitives/DrawPrimitives.hpp"
#include "Utility/Asset/PixelShaderAsset.hpp"
#include "Utility/Asset/VertexShaderAsset.hpp"
#include "Utility/MeshData.hpp"
#include "Graphics/DX11/ConstantBuffer/DX11ConstantBuffer.hpp"

#ifdef _WIN32

#include <d3d11.h>

namespace Simple
{

	class DX11WireSphereDrawer final
	{
	public:

		DX11WireSphereDrawer() = default;

		void Init(ID3D11Device& device, class AssetManager& assetManager);

		void Render(const DrawSphere& sphere, ID3D11DeviceContext& context);

	private:

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;

		MeshData<Vertex> mMeshData;
		DX11ConstantBuffer<TransformBufferData> mTransformBuffer;
		PixelShaderAssetHandle mPixelShader;
		VertexShaderAssetHandle mVertexShader;
	};
}

#endif