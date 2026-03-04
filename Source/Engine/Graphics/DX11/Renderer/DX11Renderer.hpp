#pragma once

#ifdef _WIN32

#include "Engine/Win/WinDefines.hpp"

#include "Engine/Graphics/DX11/Renderer/DX11WireBoxDrawer.hpp"
#include "Engine/Graphics/DX11/Renderer/DX11LineDrawer.hpp"
#include "Engine/Graphics/DX11/Renderer/DX11WireSphereDrawer.hpp"
#include "Engine/Graphics/DX11/Sprite/DX11SpriteDrawer.hpp"
#include "Engine/Graphics/DX11/Renderer/DX11MeshRenderer.hpp"
#include "Engine/Graphics/DX11/Text/DX11TextRenderer.hpp"
#include "Engine/Graphics/ConstantBufferData.hpp"
#include "Engine/Graphics/SkyBox/SkyBox.hpp"
#include "Engine/Graphics/RenderState.hpp"

#include <wrl/client.h>
#include <d3d11.h>

namespace CLX
{
	class AssetManager;
	class DX11RenderTargetManager;

	class DX11Renderer final
	{
	public:

		DX11Renderer(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

		void Init(AssetManager& assetManager);

		void Render(RenderState& renderState, 
			AssetManager& assetManager, 
			PixelShaderAssetHandle pixelShader,
			VertexShaderAssetHandle vertexShader,
			DX11ConstantBuffer<ColorBufferData>& colorCB, 
			DX11ConstantBuffer<TransformBufferData>& transformCB,
			DX11ConstantBuffer<ObjectIDBufferData>& objectIDCB,
			DX11ConstantBuffer<BoneBufferData>& boneBuffer,
			DX11SamplerState& samplerState);

	private:

		void RenderDebugLines(const RenderList& renderList, AssetManager& assetManager, VertexShaderAssetHandle vertexShader, DX11ConstantBuffer<ColorBufferData>& colorCB);


	private:
		
		DX11LineDrawer mLineDrawer;
		DX11WireSphereDrawer mWireSphereDrawer;
		DX11SpriteDrawer mSpriteDrawer;
		DX11WireBoxDrawer mWireBoundingBoxDrawer;

		DX11MeshRenderer mPlaneRenderer;
		DX11MeshRenderer mPyramidRenderer;
		DX11MeshRenderer mCylinderRenderer;
		DX11MeshRenderer mArrowRenderer;
		DX11MeshRenderer mSphereRenderer;
		DX11MeshRenderer mBoxRenderer;
		DX11TextRenderer mTextRenderer;

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
	};
}

#endif