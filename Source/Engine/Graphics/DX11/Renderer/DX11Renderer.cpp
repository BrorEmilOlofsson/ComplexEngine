#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11Renderer.hpp"
#include "Engine/Math/Math.hpp"
#include "Engine/Utility/Algorithm.hpp"
#include "Engine/Graphics/Model/Factory/ShapeCreator3000.hpp"
#include "Engine/Asset/AssetManager.hpp"
#include "Engine/Graphics/Mesh/Mesh.hpp"
#include "Engine/Graphics/Model/Model.hpp"
#include "Engine/Graphics/Model/AnimatedModel.hpp"
#include "Engine/Graphics/Texture/Texture.hpp"
#include "Engine/Utility/EngineDirectories.hpp"
#include "Engine/Graphics/DX11/RenderTarget/DX11RenderTarget.hpp"
#include "Engine/Graphics/DX11/DX11GBuffer.hpp"
#include "Engine/Graphics/DX11/DX11RenderContext.hpp"

namespace CLX
{

	static void RenderModels(std::span<const ModelInstance> modelInstances, DX11ConstantBuffer<TransformBufferData>& transformCB, DX11ConstantBuffer<ObjectIDBufferData>& objectIDCB, ID3D11DeviceContext& context)
	{
		for (auto& modelInstance : modelInstances)
		{
			if (!modelInstance.mesh)
			{

				if (!modelInstance.model)
				{
					continue;
				}

				if (modelInstance.albedoTexture)
				{
					modelInstance.albedoTexture->Bind();
				}

				transformCB.UpdateAndBind(TransformBufferData{ modelInstance.transform.GetMatrix() }, context);
				objectIDCB.UpdateAndBind(ObjectIDBufferData{ modelInstance.objectID }, context);

				modelInstance.model->Render();

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

	static void RenderAnimatedModels(std::span<const AnimatedModelInstance> modelInstances, DX11ConstantBuffer<TransformBufferData>& transformCB, DX11ConstantBuffer<ObjectIDBufferData>& objectIDCB, DX11ConstantBuffer<BoneBufferData>& boneCB, ID3D11DeviceContext& context)
	{
		for (auto& modelInstance : modelInstances)
		{

			if (!modelInstance.animatedModel)
			{
				continue;
			}

			if (modelInstance.albedoTexture)
			{
				modelInstance.albedoTexture->Bind();
			}

			transformCB.UpdateAndBind(TransformBufferData{ modelInstance.transform.GetMatrix() }, context);
			objectIDCB.UpdateAndBind(ObjectIDBufferData{ modelInstance.objectID }, context);

			/*for (std::size_t i = 0; i < modelInstance.boneMatrices.size(); i++)
			{
				modelInstance.boneMatrices[i] = Matrix4x4f::Identity();
			}*/

			boneCB.UpdateAndBind(BoneBufferData{ modelInstance.boneMatrices }, context);

			modelInstance.animatedModel->Render();

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

	void DX11Renderer::Render(RenderState& renderState,
		AssetManager& assetManager,
		PixelShaderAssetHandle, 
		VertexShaderAssetHandle vertexShader,
		DX11ConstantBuffer<ColorBufferData>& colorCB,
		DX11ConstantBuffer<TransformBufferData>& transformCB,
		DX11ConstantBuffer<ObjectIDBufferData>& objectIDCB, 
		DX11ConstantBuffer<BoneBufferData>& boneBuffer,
		DX11SamplerState& samplerState)
	{
		PROFILER_FUNCTION(profiler::colors::Red);

		DX11RenderContext* r = renderState.GetRenderContext()->GetUnderlying<DX11RenderContext>();

		const Vector2ui size = Vector2ui(renderState.GetRenderRect()->GetExtent());

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
		assetManager.GetVertexShader(GetShaderPath("AnimatedVS"))->Bind();
		RenderAnimatedModels(renderState.GetRenderList().GetAnimatedModelInstances(), transformCB, objectIDCB, boneBuffer, *mDeviceContext.Get());

		objectIDCB.Update(ObjectIDBufferData{}, *mDeviceContext.Get());


		// (Optional safety) Unbind MRTs before using them as SRVs
		ID3D11RenderTargetView* nullRTVs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = {};
		mDeviceContext->OMSetRenderTargets(_countof(nullRTVs), nullRTVs, nullptr);

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

		mTextRenderer.Render(renderState.GetRenderList().GetText3Ds(), *renderState.GetCamera(), size);

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
                RotationMatrix3f rotation = RotationMatrix3f::FromY(drawPyramid.direction);
                Vector3f scale = Vector3f(drawPyramid.thickness, drawPyramid.thickness, drawPyramid.thickness);

				return Matrix4x4f::CreateTRSMatrix(drawPyramid.startPoint, rotation, scale);
			});

		mCylinderRenderer.Render(*mDeviceContext.Get(), unlitPixelShader, vertexShader, renderList.GetCylinders(), colorCB,
			[](const DrawCylinder& drawCylinder)
			{
				RotationMatrix3f matrix = RotationMatrix3f::FromY(drawCylinder.cylinder.GetAxis());
				Point3f position = drawCylinder.cylinder.GetLowerPoint();
				Vector3f scale = Vector3f(drawCylinder.cylinder.GetRadius().Value() * 2, drawCylinder.cylinder.GetHeight(), drawCylinder.cylinder.GetRadius().Value() * 2);
				return Matrix4x4f::CreateTRSMatrix(position, matrix, scale);
			});

		mPlaneRenderer.Render(*mDeviceContext.Get(), unlitPixelShader, vertexShader, renderList.GetPlanes(), colorCB,
			[](const DrawPlane& drawPlane)
			{
				RotationMatrix3f matrix = RotationMatrix3f::FromY(drawPlane.plane.GetNormal());
				Vector3f scale = Vector3f(10000, 10000, 10000);
				Point3f position = drawPlane.plane.GetPoint();
				return Matrix4x4f::CreateTRSMatrix(position, matrix, scale);
			}
		);

		mArrowRenderer.Render(*mDeviceContext.Get(), unlitPixelShader, vertexShader, renderList.GetArrows(), colorCB,
			[](const DrawArrow& drawArrow)
			{
				RotationMatrix3f matrix = RotationMatrix3f::FromY(GetUnitVector(drawArrow.startPoint, drawArrow.endPoint));
				Point3f position = drawArrow.startPoint;
				Vector3f scale = Vector3f(drawArrow.thickness, Distance(drawArrow.startPoint, drawArrow.endPoint), drawArrow.thickness);
				return  Matrix4x4f::CreateTRSMatrix(position, matrix, scale);
			});

		mSphereRenderer.Render(*mDeviceContext.Get(), unlitPixelShader, vertexShader, renderList.GetSpheres(), colorCB,
			[](const DrawSphere& sphere)
			{
				Matrix4x4f matrix = Matrix4x4f::Identity();
				const float radius = sphere.sphere.GetRadius().Value();
				matrix.SetScale(Vector3f(radius, radius, radius));
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