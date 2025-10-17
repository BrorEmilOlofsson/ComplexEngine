#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11Renderer.hpp"
#include "Utility/Math/Math.hpp"
#include "Utility/Algorithm.hpp"
#include "Utility/File/FileUtility.hpp"
#include "Graphics/Model/Factory/ShapeCreator3000.hpp"
#include "Utility/Asset/AssetManager.hpp"
#include "Graphics/Mesh/Mesh.hpp"
#include "Graphics/Texture/Texture.hpp"
#include "Utility/EngineDirectories.hpp"
#include "Graphics/DX11/RenderTarget/DX11RenderTarget.hpp"
#include "Graphics/DX11/DX11GBuffer.hpp"
#include "Graphics/DX11/RenderTarget/DX11RenderTargetManager.hpp"
#include <fstream>
#include <cassert>

namespace Simple
{

	//void RenderModel(const Transform& transform, DX11ConstantBuffer<TransformBufferData>& transformBuffer, MeshAssetHandle mesh, ID3D11DeviceContext& context)
	//{
	//	TransformBufferData objectBuffer
	//	{
	//		transform.GetMatrix()
	//	};

	//	transformBuffer.UpdateAndBind(objectBuffer, context);

	//	mesh->Render();
	//}

	//void RenderModels(const RenderList& renderList, DX11ConstantBuffer<TransformBufferData>& transformBuffer, ID3D11DeviceContext& context)
	//{
	//	for (auto& r : renderList.mUnlitStaticModels)
	//	{
	//		r.pixelShader->Bind();
	//		r.vertexShader->Bind();
	//		RenderModel(r.transform, transformBuffer, r.mesh, context);
	//	}

	//	/*for (auto& r : renderList.myStaticModelsToRender)
	//	{
	//		r.albedoTexture->Bind(myDeviceContext.Get());
	//		RenderModel(r.transform, r.mesh, myDeviceContext.Get());
	//	}*/
	//}

	/*void BindTextures(const std::span<Texture* const> textures, ID3D11DeviceContext* context)
	{
		for (size_t i = 0; i < size(textures); ++i)
		{
			if (Texture* texture = textures[i])
			{
				texture->Bind();
			}
			else
			{
				static ID3D11ShaderResourceView* nullview[1] = { nullptr };
				context->PSSetShaderResources(static_cast<unsigned int>(i), 1, nullview);
			}
		}
	}*/

	static void RenderModels(std::span<const ModelInstance> models, DX11ConstantBuffer<TransformBufferData>& transformCB, ID3D11DeviceContext& context)
	{
		for (auto& modelInstance : models)
		{
			if (!modelInstance.mesh)
			{
				continue;
			}
			if (modelInstance.albedoTexture)
			{
				modelInstance.albedoTexture->Bind();
			}
			transformCB.UpdateAndBind(TransformBufferData{ modelInstance.transform.GetMatrix() }, context);
			modelInstance.mesh->Render();
		}
	}

	DX11Renderer::DX11Renderer(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
		: mDevice(device)
		, mDeviceContext(context)
	{
	}

	void DX11Renderer::Init(AssetManager& assetManager)
	{
		mLineDrawer.Init(*mDevice.Get(), assetManager);
		mWireSphereDrawer.Init(*mDevice.Get(), assetManager);
		mSpriteDrawer.Init(*mDevice.Get(), *mDeviceContext.Get(), assetManager.GetTexture(GetPathByTextureType(eTextureType::Default_Albedo)));
		mPlaneRenderer.Init(*mDevice.Get(), ShapeCreator3000::CreatePlane());
		mPyramidRenderer.Init(*mDevice.Get(), ShapeCreator3000::CreatePyramidNew());
		mArrowRenderer.Init(*mDevice.Get(), ShapeCreator3000::CreateArrow());
		mCylinderRenderer.Init(*mDevice.Get(), ShapeCreator3000::CreateCylinder());
		mSphereRenderer.Init(*mDevice.Get(), ShapeCreator3000::CreateSphere());
		mBoxRenderer.Init(*mDevice.Get(), ShapeCreator3000::CreateCube());

		mWireBoundingBoxDrawer.Init(*mDevice.Get(), assetManager);
		mTextRenderer.Init(mDeviceContext.Get(), mDevice.Get(), ToWString(Directory::Assets) + std::wstring(L"Fonts/arial.spritefont"));
	}

	static std::filesystem::path GetShaderPath(std::string_view name)
	{
		return std::filesystem::path(SIMPLE_DIR_SHADERS) / (std::string(name) + ".cso");
	}

	void DX11Renderer::Render(const RenderState& renderState, AssetManager& assetManager,
		PixelShaderAssetHandle pixelShader, VertexShaderAssetHandle vertexShader,
		DX11ConstantBuffer<ColorBufferData>& colorCB, DX11ConstantBuffer<TransformBufferData>& transformCB,
		DX11RenderTargetManager& renderTargetManager, DX11SamplerState& samplerState)
	{
		transformCB;
		vertexShader;
		pixelShader;
		colorCB;
		assetManager;
		PROFILER_FUNCTION(profiler::colors::Red);

		const Vector2ui size = Vector2ui(renderState.GetRenderRect().value_or(AABB2i::CreateFromMinAndExtent(Point2i::Zero(), Vector2i(1600, 900))).GetExtent());

		auto viewport = DX11Factory::CreateViewport(size);
		mDeviceContext->RSSetViewports(1, &viewport);

		static bool firstTime = true;
		static std::unique_ptr<DX11GBuffer> gBuffer = std::make_unique<DX11GBuffer>(mDeviceContext, mDevice);
		if (firstTime)
		{
			gBuffer->Init(size);
			firstTime = false;
		}

		gBuffer->Clear();

		if (gBuffer->GetSize() != size)
		{
			gBuffer->Resize(size);
		}

		mDeviceContext->OMSetRenderTargets(
			static_cast<UINT>(gBuffer->GetRTVArray().size()),
			gBuffer->GetRTVArray().data(),
			gBuffer->GetDepthStencilView()
		);

		assetManager.GetPixelShader(GetShaderPath("GBufferPS"))->Bind();
		assetManager.GetVertexShader(GetPath(eVertexShaderType::Default))->Bind();

		RenderModels(renderState.GetRenderList().GetModelInstances(), transformCB, *mDeviceContext.Get());

		RenderDebugLines(renderState.GetRenderList(), assetManager, pixelShader, vertexShader, colorCB);

		mTextRenderer.Render(renderState.GetRenderList().GetText3Ds(), *renderState.GetCamera(), size);
		
		// (Optional safety) Unbind MRTs before using them as SRVs
		ID3D11RenderTargetView* nullRTVs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = {};
		mDeviceContext->OMSetRenderTargets(_countof(nullRTVs), nullRTVs, nullptr);

		auto rtv = renderTargetManager.Get(renderState.GetRenderTargetView().value())->GetRenderTargetView();

		mDeviceContext->OMSetRenderTargets(1, &rtv, nullptr);
		ID3D11ShaderResourceView* dummy[4] = {};
		mDeviceContext->PSSetShaderResources(5, 4, dummy); // Clear old

		mDeviceContext->PSSetShaderResources(
			TextureSlots::GBufferStart, // 5
			static_cast<UINT>(gBuffer->GetSRVArray().size()),
			gBuffer->GetSRVArray().data()
		);

		RenderFullScreen(
			*mDeviceContext.Get(),
			renderState.GetRenderTargetView().value(),
			samplerState,
			assetManager.GetVertexShader(GetShaderPath("FullScreenQuadVS")),
			assetManager.GetPixelShader(GetShaderPath("DeferredLightingPS"))
		);
	}

	void DX11Renderer::RenderDebugLines(const RenderList& renderList, AssetManager& assetManager, PixelShaderAssetHandle pixelShader,
		VertexShaderAssetHandle vertexShader, DX11ConstantBuffer<ColorBufferData>& colorCB)
	{
		PROFILER_FUNCTION(profiler::colors::Gold);

		mLineDrawer.Render(renderList.GetLines(), *mDeviceContext.Get());

		PixelShaderAssetHandle unlitPixelShader = assetManager.GetPixelShader(GetPath(ePixelShaderType::UnlitDefault));

		for (const DrawSphere& drawSphere : renderList.GetWireSpheres())
		{
			mWireSphereDrawer.Render(drawSphere, *mDeviceContext.Get());
		}

		for (const DrawBoundingBox& drawBox : renderList.GetWireBoundingBoxes())
		{
			mWireBoundingBoxDrawer.Render(drawBox.boundingBox, drawBox.modelToWorld.GetMatrix(), drawBox.color, *mDeviceContext.Get());
		}

		mPyramidRenderer.Render(*mDeviceContext.Get(), pixelShader, vertexShader, renderList.GetPyramids(), colorCB,
			[](const DrawPyramid& drawPyramid)
			{
				Matrix4x4f matrix = CreateMatrixFromY(drawPyramid.direction);
				matrix.SetTranslation(drawPyramid.startPoint);
				matrix.SetScale(Vector3f(drawPyramid.thickness, drawPyramid.thickness, drawPyramid.thickness));
				return matrix;
			});

		mCylinderRenderer.Render(*mDeviceContext.Get(), unlitPixelShader, vertexShader, renderList.GetCylinders(), colorCB,
			[](const DrawCylinder& drawCylinder)
			{
				Matrix4x4f matrix = CreateMatrixFromY(drawCylinder.cylinder.GetAxis());
				matrix.SetTranslation(drawCylinder.cylinder.GetLowerPoint());
				matrix.SetScale(Vector3f(drawCylinder.cylinder.GetRadius() * 2, drawCylinder.cylinder.GetHeight(), drawCylinder.cylinder.GetRadius() * 2));
				return matrix;
			});

		mPlaneRenderer.Render(*mDeviceContext.Get(), unlitPixelShader, vertexShader, renderList.GetPlanes(), colorCB,
			[](const DrawPlane& drawPlane)
			{
				Matrix4x4f matrix = CreateMatrixFromY(drawPlane.plane.GetNormal());
				matrix.SetScale(Vector3f(10000, 10000, 10000));
				matrix.SetTranslation(drawPlane.plane.GetPoint());
				return matrix;
			}
		);

		mArrowRenderer.Render(*mDeviceContext.Get(), unlitPixelShader, vertexShader, renderList.GetArrows(), colorCB,
			[](const DrawArrow& drawArrow)
			{
				Matrix4x4f matrix = CreateMatrixFromY(GetUnitVector(drawArrow.startPoint, drawArrow.endPoint));
				matrix.SetTranslation(drawArrow.startPoint);
				matrix.SetScale(Vector3f(drawArrow.thickness, Distance(drawArrow.startPoint, drawArrow.endPoint), drawArrow.thickness));
				return matrix;
			});

		mSphereRenderer.Render(*mDeviceContext.Get(), unlitPixelShader, vertexShader, renderList.GetSpheres(), colorCB,
			[](const DrawSphere& sphere)
			{
				Matrix4x4f matrix = Matrix4x4f::Identity();
				matrix.SetScale(Vector3f(sphere.sphere.GetRadius(), sphere.sphere.GetRadius(), sphere.sphere.GetRadius()));
				matrix.SetTranslation(sphere.sphere.GetCenter());
				return matrix;
			}
		);

		mBoxRenderer.Render(*mDeviceContext.Get(), unlitPixelShader, vertexShader, renderList.GetBoundingBoxes(), colorCB,
			[](const DrawBoundingBox& boundingBox)
			{
				return boundingBox.modelToWorld.GetMatrix();
			}
		);
	}
}