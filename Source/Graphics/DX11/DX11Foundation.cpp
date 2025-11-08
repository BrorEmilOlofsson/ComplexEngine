#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11Foundation.hpp"
#include "Graphics/DX11/DX11Factory.hpp"
#include "Graphics/DX11/Texture/DX11Texture.hpp"
#include "Graphics/Texture/Texture.hpp"
#include "Graphics/Mesh/Mesh.hpp"
#include "Graphics/Model/Model.hpp"
#include "Graphics/Model/AnimatedModel.hpp"
#include "Graphics/DX11/DX11LoadFunctions.hpp"
#include "Graphics/Model/Factory/ShapeCreator3000.hpp"
#include "Graphics/DX11/Shader/DX11PixelShader.hpp"
#include "Graphics/DX11/Shader/DX11VertexShader.hpp"
#include "Graphics/DX11/DX11RenderContext.hpp"
#include "Graphics/Shader/PixelShader.hpp"
#include "Graphics/Shader/VertexShader.hpp"

namespace Simple
{

	void CreateLoaders(AssetLoader& assetLoader, Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{

		auto textureLoader = [context, device](const std::filesystem::path& path) -> TextureAsset
			{
				Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = DX11Factory::LoadDDS(*device.Get(), *context.Get(), path);
				return TextureAsset(std::make_shared<Texture>(DX11Texture(srv, context, std::filesystem::relative(path))));
			};

		auto meshLoader = [device, context](const std::filesystem::path& path) -> MeshAsset
			{
				std::expected<DX11Mesh, std::string> meshResult = LoadDX11Mesh(path, *device.Get(), context);
				if (meshResult.has_value())
				{
					return MeshAsset(std::make_shared<Mesh>(std::move(meshResult.value())));
				}
				return MeshAsset::Empty();
			};

		auto modelLoader = [device, context](const std::filesystem::path& path) -> std::variant<std::monostate, ModelAsset, AnimatedModelAsset>
			{
				std::expected<std::variant<DX11Model, DX11AnimatedModel>, std::string> meshResult = LoadDX11Model(path, *device.Get(), context);
				if (meshResult.has_value())
				{
					if (DX11Model* model = std::get_if<DX11Model>(&meshResult.value()))
					{
						return ModelAsset(std::make_shared<Model>(std::move(*model)));
					}
					else if (DX11AnimatedModel* animatedModel = std::get_if<DX11AnimatedModel>(&meshResult.value()))
					{
						return AnimatedModelAsset(std::make_shared<AnimatedModel>(std::move(*animatedModel)));
					}
				}
				return std::monostate{};
			};

		auto pixelShaderLoader = [device, context](const std::filesystem::path& path) -> PixelShaderAsset
			{
				auto ps = DX11Factory::CreatePixelShader(path, *device.Get());
				return PixelShaderAsset(std::make_shared<PixelShader>(DX11PixelShader(device, context, ps, path)));
			};

		auto vertexShaderLoader = [device, context](const std::filesystem::path& path) -> VertexShaderAsset
			{
				Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
				auto dx11Vs = DX11Factory::CreateVertexShaderAndInputLayout(path, *device.Get(), inputLayout);
				return VertexShaderAsset(std::make_shared<VertexShader>(DX11VertexShader(device, context, dx11Vs, inputLayout)));
			};

		assetLoader.SetTextureLoader(std::move(textureLoader));
		assetLoader.SetMeshLoader(std::move(meshLoader));
		assetLoader.SetModelLoader(std::move(modelLoader));
		assetLoader.SetPixelShaderLoader(std::move(pixelShaderLoader));
		assetLoader.SetVertexShaderLoader(std::move(vertexShaderLoader));
	}

	static std::pair<Microsoft::WRL::ComPtr<ID3D11Device>, Microsoft::WRL::ComPtr<ID3D11DeviceContext>> CreateDeviceAndContext()
	{
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
		DX11Factory::CreateDeviceAndContext(device, context);
		return { device, context };
	}

	DX11Foundation::DX11Foundation()
		: mDeviceAndContext(CreateDeviceAndContext())
		, mImGui(GetDevice().Get(), GetContext().Get())
		, mConstantBufferManager(*GetDevice().Get())
		, mRenderer(GetDevice(), GetContext())
		, mRenderTargetManager(GetDevice(), GetContext())
		, mDepthStencilViewManager(std::make_shared<DX11DepthStencilViewManager>(GetDevice()))
		, mSamplerState(std::make_shared<DX11SamplerState>(*GetDevice().Get()))
	{
	}

	void DX11Foundation::BeginFrame(const GraphicsBufferData& bufferData)
	{
#ifdef _EDITOR
		mImGui.BeginFrame();
#endif

		mFrameTimer = bufferData.frameTimer;
		mTotalTimer = bufferData.totalTimer;

		const float totalTime = static_cast<float>(mTotalTimer.GetDeltaTime().count());
		const float deltaTime = mFrameTimer.GetDeltaTime().count();

		mConstantBufferManager.UpdateTimeBuffer(totalTime, deltaTime, *GetContext().Get());
		mConstantBufferManager.UpdatePostProcessBuffer(PostProcessBufferData{}, *GetContext().Get());

		mRenderTargetManager.ClearAll(mGraphicsSettings->clearColor);

		for (auto& depthStencilView : *mDepthStencilViewManager)
		{
			GetContext()->ClearDepthStencilView(depthStencilView.second.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}

	}

	void DX11Foundation::EndFrame()
	{
#ifdef _EDITOR
		mImGui.EndFrame();
#endif
	}

	void DX11Foundation::Init()
	{
		mImGui.Load();
		mRenderer.Init(*mAssetManager);
	}

	void DX11Foundation::Shutdown()
	{
		mImGui.Save();
	}

	static void ValidateRenderState(const RenderState& renderState)
	{
		if (!renderState.GetCamera())
		{
			throw std::runtime_error("No camera set in RenderState");
		}

		if (renderState.GetRenderContext() == nullptr)
		{
			throw std::runtime_error("No render context set in RenderState");
		}

		if (!renderState.GetRenderRect().has_value())
		{
			throw std::runtime_error("No render rect set in RenderState");
		}

		if (renderState.GetRenderRect().value().GetExtent() == Vector2i::Zero())
		{
			throw std::runtime_error("Render rect is too small");
		}

		Camera camera = renderState.GetCamera().value();
		Matrix4x4f proj = camera.GetProjectionMatrix();
		Vector2i resolution = renderState.GetRenderRect()->GetExtent();
		Matrix4x4f expected = Camera::CreatePerspectiveProjectionMatrix(camera.GetHorizontalFOV(), camera.GetNearPlane(), camera.GetFarPlane(), Vector2ui(resolution));
		if (proj != expected)
		{
			//throw std::runtime_error("Resolution in camera does not correspond to RenderState resolution");
		}
	}

	static PointLightBufferData CreatePointLightData(std::span<const PointLight> pointLights)
	{
		auto toPointLightData = [](const PointLight& pointLight)
			{
				PointLightData data;
				data.color = pointLight.color;
				data.intensity = pointLight.intensity;
				data.position = pointLight.position;
				data.range = pointLight.range;
				return data;
			};
		PointLightBufferData data;
		const std::size_t count = Min(pointLights.size(), data.pointLights.size());
		std::ranges::transform(pointLights | std::views::take(count), data.pointLights.begin(), toPointLightData);
		data.used = static_cast<unsigned int>(count);
		return data;
	}

	static LightBufferData CreateLightData(const RenderState& renderState)
	{
		LightBufferData lightData;
		if (renderState.GetDirectionalLight())
		{
			const DirectionalLight& dirLight = renderState.GetDirectionalLight().value();
			lightData.directionalLightDirection = ToVector3(dirLight.direction);
			lightData.directionalLightColor = dirLight.color;
			lightData.directionalLightIntensity = dirLight.intensity;
		}
		else
		{
			lightData.directionalLightColor = Colors::Black;
		}
		if (renderState.GetAmbientLight())
		{
			lightData.ambientLightColor = renderState.GetAmbientLight()->GetColor();
			lightData.ambientLightIntensity = renderState.GetAmbientLight()->GetIntensity();
		}
		return lightData;
	}

	void DX11Foundation::Render(RenderState& renderState)
	{
		PROFILER_FUNCTION(profiler::colors::Red);

		ValidateRenderState(renderState);

		const Vector2ui bufferSize = renderState.GetRenderContext()->GetBufferSize();
		const Vector2ui renderSize = Vector2ui(renderState.GetRenderRect()->GetExtent());
		if (renderSize.x == 0 || renderSize.y == 0)
		{
			return;
		}
		if (renderState.GetRenderContext()->GetBufferSize() != renderSize)
		{
			renderState.GetRenderContext()->ResizeBuffers(renderSize);

			RenderState& r = const_cast<RenderState&>(renderState);
			Camera camera = *renderState.GetCamera();
			camera.SetResolution(renderSize);
			r.SetCamera(camera);
		}

		mConstantBufferManager.UpdatePointLights(CreatePointLightData(renderState.GetRenderList().GetPointLights()), *GetContext().Get());
		mConstantBufferManager.UpdateCameraBuffer(*renderState.GetCamera(), renderSize, *GetContext().Get());


		{
			const LightBufferData lightData = CreateLightData(renderState);
			mConstantBufferManager.UpdateLightBuffer(lightData, *GetContext().Get());
		}


		mRenderer.Render(
			renderState,
			*mAssetManager,
			mAssetManager->GetPixelShader(GetPath(ePixelShaderType::LitDefault)),
			mAssetManager->GetVertexShader(GetPath(eVertexShaderType::Default)),
			mConstantBufferManager.mColorBuffer,
			mConstantBufferManager.mTransformBuffer,
			mConstantBufferManager.mObjectIDBuffer,
			*mSamplerState
		);
	}

	void DX11Foundation::SetAssetLoaders(AssetLoader& assetLoader) const
	{
		CreateLoaders(assetLoader, mDeviceAndContext.first, mDeviceAndContext.second);
	}

	static void AddMesh(AssetManager& assetManager, MeshData<Vertex>&& meshData, const std::string_view meshName, ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(DX11Mesh(meshData, meshName, meshName, device, context));
		assetManager.AddMesh(meshName, MeshAsset(mesh));
	}

	static void AddModel(AssetManager& assetManager, MeshData<Vertex>&& meshData, const std::string_view meshName, ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		std::vector<DX11Mesh> v;
		v.push_back(DX11Mesh(std::move(meshData), meshName, meshName, device, context));
		std::shared_ptr<Model> model = std::make_shared<Model>(DX11Model(std::move(v), std::string(meshName), meshName, device, context));
		assetManager.AddModel(meshName, ModelAsset(model));
	}

	std::function<void(AssetManager&)> DX11Foundation::GetDefaultAssetLoader()
	{
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = GetContext();
		Microsoft::WRL::ComPtr<ID3D11Device> device = GetDevice();

		return [context, device](AssetManager& assetManager)
			{
				AddMesh(assetManager, ShapeCreator3000::CreateCube(), EnumToString(ePrimitiveShape::Cube), *device.Get(), context);
				AddMesh(assetManager, ShapeCreator3000::CreatePyramid(), EnumToString(ePrimitiveShape::Pyramid), *device.Get(), context);
				AddMesh(assetManager, ShapeCreator3000::CreatePlane(), EnumToString(ePrimitiveShape::Plane), *device.Get(), context);
				AddMesh(assetManager, ShapeCreator3000::CreateSkyBox(Vector3f(100, 100, 100)), EnumToString(ePrimitiveShape::SkyBox), *device.Get(), context);
				AddMesh(assetManager, ShapeCreator3000::CreateSphere(), EnumToString(ePrimitiveShape::Sphere), *device.Get(), context);

				AddModel(assetManager, ShapeCreator3000::CreateCube(), EnumToString(ePrimitiveShape::Cube), *device.Get(), context);
				AddModel(assetManager, ShapeCreator3000::CreatePyramid(), EnumToString(ePrimitiveShape::Pyramid), *device.Get(), context);
				AddModel(assetManager, ShapeCreator3000::CreatePlane(), EnumToString(ePrimitiveShape::Plane), *device.Get(), context);
				AddModel(assetManager, ShapeCreator3000::CreateSkyBox(Vector3f(100, 100, 100)), EnumToString(ePrimitiveShape::SkyBox), *device.Get(), context);
				AddModel(assetManager, ShapeCreator3000::CreateSphere(), EnumToString(ePrimitiveShape::Sphere), *device.Get(), context);
			};
	}

	RenderContext DX11Foundation::CreateRenderContext(const Vector2ui& size)
	{
		DX11RenderContext dx11RenderContext(GetDevice(), GetContext(), size);

		return RenderContext(std::move(dx11RenderContext));
	}
}