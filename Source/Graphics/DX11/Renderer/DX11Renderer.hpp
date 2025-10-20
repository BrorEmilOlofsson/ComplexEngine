#pragma once
#include "Utility/Math/Matrix4x4.hpp"
#include "Graphics/DX11/Renderer/DX11WireBoxDrawer.hpp"
#include "Graphics/DX11/Renderer/DX11LineDrawer.hpp"
#include "Graphics/DX11/Renderer/DX11WireSphereDrawer.hpp"
#include "Graphics/DX11/Sprite/DX11SpriteDrawer.hpp"
#include "Graphics/DX11/Renderer/DX11MeshRenderer.hpp"
#include "Graphics/DX11/Text/DX11TextRenderer.hpp"
#include "Graphics/ConstantBufferData.hpp"
#include "Graphics/SkyBox/SkyBox.hpp"
#include "Graphics/Light/DirectionalLight.hpp"
#include "Graphics/Model/ModelInstance.hpp"
#include "Graphics/Model/AnimatedModelInstance.hpp"
#include "Graphics/Model/Model.hpp"
#include "Graphics/RenderState.hpp"
#include <memory>
#include <vector>

#ifdef _WIN32

#include <wrl/client.h>
#include <d3d11.h>

namespace Simple
{
	class AssetManager;
	class DX11RenderTargetManager;

	class DX11Renderer final
	{
	public:

		DX11Renderer(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

		void Init(AssetManager& assetManager);
		void Render(RenderState& renderState, AssetManager& assetManager, 
			PixelShaderAssetHandle pixelShader, VertexShaderAssetHandle vertexShader,
			DX11ConstantBuffer<ColorBufferData>& colorCB, DX11ConstantBuffer<TransformBufferData>& transformCB, DX11ConstantBuffer<ObjectIDBufferData>& objectIDCB,
			DX11SamplerState& samplerState);

	private:

		void RenderDebugLines(const RenderList& renderList, AssetManager& assetManager,
			PixelShaderAssetHandle pixelShader, VertexShaderAssetHandle vertexShader, DX11ConstantBuffer<ColorBufferData>& colorCB);


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