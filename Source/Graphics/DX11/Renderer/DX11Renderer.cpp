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
#include "Utility/ShapeMath.hpp"
#include "Graphics/DX11/DX11RenderContext.hpp"
#include <fstream>
#include <cassert>

namespace Simple
{

	static void RenderModels(std::span<const ModelInstance> models, DX11ConstantBuffer<TransformBufferData>& transformCB, DX11ConstantBuffer<ObjectIDBufferData>& objectIDCB, ID3D11DeviceContext& context)
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
			objectIDCB.UpdateAndBind(ObjectIDBufferData{ modelInstance.objectID }, context);
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


	static constexpr Point2i MapToRenderRect(const Point2i mouseScreenPos, const AABB2i& renderRect)
	{
		const AABB2f windowRect = AABB2f::CreateFromMinAndExtent(Point2f::Zero(), Vector2f(renderRect.GetExtent()));
		const Point2i mappedPos = Point2i(Remap(Point2f(mouseScreenPos), ToAABB<float>(renderRect), windowRect));
		return mappedPos;
	}

	bool IsInsideRenderRect(const Point2i& mouseScreenPos, const AABB2i& renderRect)
	{
		const AABB2f windowRect = AABB2f::CreateFromMinAndExtent(Point2f::Zero(), Vector2f(renderRect.GetExtent()));
		const Point2i mappedPos = MapToRenderRect(mouseScreenPos, renderRect);

		return IsInside(mappedPos, ToAABB<int>(windowRect));
	}

	void DX11Renderer::Render(RenderState& renderState, AssetManager& assetManager,
		PixelShaderAssetHandle, VertexShaderAssetHandle vertexShader,
		DX11ConstantBuffer<ColorBufferData>& colorCB, DX11ConstantBuffer<TransformBufferData>& transformCB, DX11ConstantBuffer<ObjectIDBufferData>& objectIDCB, DX11SamplerState& samplerState)
	{
		PROFILER_FUNCTION(profiler::colors::Red);

		DX11RenderContext* r = renderState.GetRenderContext()->GetUnderlying<DX11RenderContext>();

		const Vector2ui size = Vector2ui(renderState.GetRenderRect().value_or(AABB2i::CreateFromMinAndExtent(Point2i::Zero(), Vector2i(1600, 900))).GetExtent());

		auto viewport = DX11Factory::CreateViewport(size);
		mDeviceContext->RSSetViewports(1, &viewport);

		RenderContext& renderContext = *renderState.GetRenderContext();
		if (renderContext.GetBufferSize() != size)
		{
			renderContext.ResizeBuffers(size);
		}

		renderContext.ClearBuffers();

		

		renderContext.SetGBufferRenderTargets();

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState = DX11Factory::CreateDepthBuffer(*mDevice.Get());

		mDeviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);


		//mDeviceContext->OMSetDepthStencilState()

		assetManager.GetPixelShader(GetShaderPath("GBufferPS"))->Bind();
		assetManager.GetVertexShader(GetPath(eVertexShaderType::Default))->Bind();

		RenderModels(renderState.GetRenderList().GetModelInstances(), transformCB, objectIDCB, *mDeviceContext.Get());

		objectIDCB.Update(ObjectIDBufferData{}, *mDeviceContext.Get());

		
		// (Optional safety) Unbind MRTs before using them as SRVs
		ID3D11RenderTargetView* nullRTVs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = {};
		mDeviceContext->OMSetRenderTargets(_countof(nullRTVs), nullRTVs, nullptr);

		Point2i mouseScreenPos = renderState.mCursorScreenPos;

		if (IsInsideRenderRect(mouseScreenPos, renderState.GetRenderRect().value()))
		{
			const Point2i mappedPos = MapToRenderRect(mouseScreenPos, renderState.GetRenderRect().value());

			uint32_t id = renderContext.GetObjectIDAt(mappedPos);
			const_cast<RenderState&>(renderState).mSelectedObjectID = id;
		}
		else
		{
			const_cast<RenderState&>(renderState).mSelectedObjectID = std::numeric_limits<uint32_t>::max();
		}

		renderContext.SetOutputRenderTarget();
		ID3D11ShaderResourceView* dummy[5] = {};
		mDeviceContext->PSSetShaderResources(TextureSlots::GBufferStart, static_cast<UINT>(renderContext.GetGBufferSRVs().size()), dummy); // Clear old

		renderContext.SetGBufferShaderResources();

		RenderFullScreen(
			*mDeviceContext.Get(),
		r->GetOutputRenderTarget().GetShaderResourceView(),
			samplerState,
			assetManager.GetVertexShader(GetShaderPath("FullScreenQuadVS")),
			assetManager.GetPixelShader(GetShaderPath("DeferredLightingPS"))
		);
		

		// Render debug lines

		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;   // READ-ONLY
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthReadOnlyState;
		mDevice->CreateDepthStencilState(&dsDesc, &depthReadOnlyState);

		auto rtv = r->GetOutputRenderTarget().GetRenderTargetView();
		// Re-bind the same GBuffer depth buffer used before!
		mDeviceContext->OMSetRenderTargets(1, &rtv, r->GetGBuffer().GetDepthStencilView());
		mDeviceContext->OMSetDepthStencilState(depthReadOnlyState.Get(), 0);

		RenderDebugLines(renderState.GetRenderList(), assetManager, vertexShader, colorCB);

		//mTextRenderer.Render(renderState.GetRenderList().GetText3Ds(), *renderState.GetCamera(), size);

	}

	void DX11Renderer::RenderDebugLines(const RenderList& renderList, AssetManager& assetManager,
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

		mPyramidRenderer.Render(*mDeviceContext.Get(), unlitPixelShader, vertexShader, renderList.GetPyramids(), colorCB,
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