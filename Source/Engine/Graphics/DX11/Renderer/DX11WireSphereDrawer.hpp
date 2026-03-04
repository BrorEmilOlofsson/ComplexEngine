#pragma once

#ifdef _WIN32

#include "Engine/Win/WinDefines.hpp"

#include "Engine/Utility/Color.hpp"
#include "Engine/Math/Shapes/Sphere.hpp"
#include "Engine/Utility/Vertex.hpp"
#include "Engine/Graphics/ConstantBufferData.hpp"
#include "Engine/Utility/DrawPrimitives/DrawPrimitives.hpp"
#include "Engine/Asset/PixelShaderAsset.hpp"
#include "Engine/Asset/VertexShaderAsset.hpp"
#include "Engine/Utility/MeshData.hpp"
#include "Engine/Graphics/DX11/ConstantBuffer/DX11ConstantBuffer.hpp"

#include <wrl/client.h>
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