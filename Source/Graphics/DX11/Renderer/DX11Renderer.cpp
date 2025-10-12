#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11Renderer.hpp"
#include "Utility/Math/Math.hpp"
#include "Utility/Algorithm.hpp"
#include "Utility/File/FileUtility.hpp"
#include "Graphics/Model/Factory/ShapeCreator3000.hpp"
#include "Utility/Asset/AssetManager.hpp"
#include "Graphics/Mesh/Mesh.hpp"
#include "Graphics/Texture/Texture.hpp"
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
		mTextRenderer.Init(mDeviceContext.Get(), mDevice.Get(), L"Assets/Fonts/arial.spritefont");
	}

	void DX11Renderer::Render(const RenderState& renderState, AssetManager& assetManager,
		PixelShaderAssetHandle pixelShader, VertexShaderAssetHandle vertexShader, const Vector2ui& windowSize, 
		DX11ConstantBuffer<ColorBufferData>& colorCB, DX11ConstantBuffer<TransformBufferData>& transformCB)
	{
		PROFILER_FUNCTION(profiler::colors::Red);

		auto models = renderState.GetRenderList().GetModelInstances();

		assetManager.GetPixelShader(GetPath(ePixelShaderType::LitDefault))->Bind();
		assetManager.GetVertexShader(GetPath(eVertexShaderType::Default))->Bind();
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

			transformCB.UpdateAndBind(TransformBufferData{ modelInstance.transform.GetMatrix() }, *mDeviceContext.Get());

			modelInstance.mesh->Render();
		}


		RenderDebugLines(renderState.GetRenderList(), assetManager, pixelShader, vertexShader, colorCB);

		mTextRenderer.Render(renderState.GetRenderList().GetText3Ds(), *renderState.GetCamera(), windowSize);
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