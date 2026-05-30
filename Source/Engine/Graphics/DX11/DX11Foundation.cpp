#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11Foundation.hpp"
#include "Engine/Graphics/DX11/DX11Factory.hpp"
#include "Engine/Graphics/DX11/Texture/DX11Texture.hpp"
#include "Engine/Graphics/Texture/Texture.hpp"
#include "Engine/Graphics/Mesh/Mesh.hpp"
#include "Engine/Graphics/Model/Model.hpp"
#include "Engine/Graphics/Model/AnimatedModel.hpp"
#include "Engine/Graphics/DX11/DX11LoadFunctions.hpp"
#include "Engine/Graphics/Model/Factory/ShapeCreator3000.hpp"
#include "Engine/Graphics/DX11/Shader/DX11PixelShader.hpp"
#include "Engine/Graphics/DX11/Shader/DX11VertexShader.hpp"
#include "Engine/Graphics/DX11/DX11RenderContext.hpp"
#include "Engine/Graphics/Shader/PixelShader.hpp"
#include "Engine/Graphics/Shader/VertexShader.hpp"

namespace CLX
{

	void CreateLoaders(AssetLoader& assetLoader, Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{

		auto textureLoader = [context, device](const std::filesystem::path& path) -> TextureAsset
			{
				Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = DX11Factory::LoadDDS(*device.Get(), *context.Get(), path);
				return TextureAsset(Texture(DX11Texture(srv, context)), std::filesystem::relative(path));
			};

		auto meshLoader = [device, context](const std::filesystem::path& path) -> MeshAsset
			{
				std::expected<DX11Mesh, std::string> meshResult = LoadDX11Mesh(path, *device.Get(), context);
				if (meshResult.has_value())
				{
					return MeshAsset(Mesh(std::move(meshResult.value())), path);
				}
				return MeshAsset::Empty();
			};

		auto fbxLoader = [device, context](const std::filesystem::path& path) -> FBXParseResult
			{
				FBXParseResult result;
				std::expected<FBXResult, std::string> meshResult = LoadDX11FBX(path, *device.Get(), context);
				if (meshResult.has_value())
				{
					if (DX11Model* model = std::get_if<DX11Model>(&meshResult.value().model))
					{
						result.model = ModelAsset(Model(std::move(*model)), path);
					}
					else if (DX11AnimatedModel* animatedModel = std::get_if<DX11AnimatedModel>(&meshResult.value().model))
					{
						result.model = AnimatedModelAsset(AnimatedModel(std::move(*animatedModel)), path);
					}
					else
					{
						result.model = std::monostate{};
					}

					if (!meshResult.value().animations.empty())
					{
						for (auto& animation : meshResult.value().animations)
						{
							result.animations.push_back(AnimationAsset(Animation(std::move(animation)), path));
						}
					}
				}

				return result;
			};

		auto pixelShaderLoader = [device, context](const std::filesystem::path& path) -> PixelShaderAsset
			{
				auto ps = DX11Factory::CreatePixelShader(path, *device.Get());
				return PixelShaderAsset(PixelShader(DX11PixelShader(device, context, ps)), path);
			};

		auto vertexShaderLoader = [device, context](const std::filesystem::path& path) -> VertexShaderAsset
			{
				Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
				auto dx11Vs = DX11Factory::CreateVertexShaderAndInputLayout(path, *device.Get(), inputLayout);
				return VertexShaderAsset(VertexShader(DX11VertexShader(device, context, dx11Vs, inputLayout)), path);
			};

		assetLoader.SetTextureLoader(std::move(textureLoader));
		assetLoader.SetMeshLoader(std::move(meshLoader));
		assetLoader.SetFBXLoader(std::move(fbxLoader));
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

		mRenderTargetManager.ClearAll(mGraphicsSettings.lock()->clearColor);

		for (auto& depthStencilView : *mDepthStencilViewManager)
		{
			GetContext()->ClearDepthStencilView(depthStencilView.second.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}

	}

	void DX11Foundation::EndFrame()
	{
		for (auto& window : mWindows)
		{
			window.second.BindBackBuffer();
		}
#ifdef _EDITOR
		mImGui.EndFrame();
#endif
	}

	void DX11Foundation::Init()
	{
		mImGui.Load();
		mRenderer.Init(*mAssetManager.lock());
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

		if (renderState.GetRenderRect().value().GetExtent() == Vector2u::Zero())
		{
			throw std::runtime_error("Render rect is too small");
		}

		Camera camera = renderState.GetCamera().value();
		Matrix4x4f proj = camera.GetProjectionMatrix();
		Dimension2u resolution = GetDimension(*renderState.GetRenderRect());
		Matrix4x4f expected = Camera::CreatePerspectiveProjectionMatrix(camera.GetHorizontalFOV(), camera.GetNearPlane(), camera.GetFarPlane(), ToAspectRatio(resolution));
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

		const Dimension2u bufferSize = renderState.GetRenderContext()->GetBufferSize();
		const Dimension2u renderSize = GetDimension(*renderState.GetRenderRect());
		if (renderSize.GetWidth() == 0 || renderSize.GetHeight() == 0)
		{
			return;
		}
		if (renderState.GetRenderContext()->GetBufferSize() != renderSize)
		{
			renderState.GetRenderContext()->ResizeBuffers(renderSize);

			RenderState& r = const_cast<RenderState&>(renderState);
			Camera camera = *renderState.GetCamera();
			camera.SetAspectRatio(ToAspectRatio(renderSize));
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
			*mAssetManager.lock(),
			mAssetManager.lock()->GetPixelShader(GetPath(ePixelShaderType::LitDefault)),
			mAssetManager.lock()->GetVertexShader(GetPath(eVertexShaderType::Default)),
			mConstantBufferManager.mColorBuffer,
			mConstantBufferManager.mTransformBuffer,
			mConstantBufferManager.mObjectIDBuffer,
			mConstantBufferManager.mBoneBuffer,
			*mSamplerState
		);
	}

	void DX11Foundation::SetAssetLoaders(AssetLoader& assetLoader) const
	{
		CreateLoaders(assetLoader, mDeviceAndContext.first, mDeviceAndContext.second);
	}

	static void AddMesh(AssetManager& assetManager, MeshData<Vertex>&& meshData, const std::string_view meshName, ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		Mesh mesh = Mesh(DX11Mesh(meshData, meshName, meshName, device, context));
		assetManager.AddMesh(meshName, MeshAsset(std::move(mesh), meshName));
	}

	static void AddModel(AssetManager& assetManager, MeshData<Vertex>&& meshData, const std::string_view meshName, ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		std::vector<DX11Mesh> v;
		v.push_back(DX11Mesh(std::move(meshData), meshName, meshName, device, context));
		Model model = Model(DX11Model(std::move(v), std::string(meshName), meshName, device, context));
		assetManager.AddModel(meshName, ModelAsset(std::move(model), meshName));
	}

	std::function<void(AssetManager&)> DX11Foundation::GetDefaultAssetLoader()
	{
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = GetContext();
		Microsoft::WRL::ComPtr<ID3D11Device> device = GetDevice();

		return [context, device](AssetManager& assetManager)
			{
				AddMesh(assetManager, ShapeCreator3000::CreateCube(), ToString(ePrimitiveShape::Cube), *device.Get(), context);
				AddMesh(assetManager, ShapeCreator3000::CreatePyramid(), ToString(ePrimitiveShape::Pyramid), *device.Get(), context);
				AddMesh(assetManager, ShapeCreator3000::CreatePlane(), ToString(ePrimitiveShape::Plane), *device.Get(), context);
				AddMesh(assetManager, ShapeCreator3000::CreateSkyBox(Vector3f(100, 100, 100)), ToString(ePrimitiveShape::SkyBox), *device.Get(), context);
				AddMesh(assetManager, ShapeCreator3000::CreateSphere(), ToString(ePrimitiveShape::Sphere), *device.Get(), context);

				AddModel(assetManager, ShapeCreator3000::CreateCube(), ToString(ePrimitiveShape::Cube), *device.Get(), context);
				AddModel(assetManager, ShapeCreator3000::CreatePyramid(), ToString(ePrimitiveShape::Pyramid), *device.Get(), context);
				AddModel(assetManager, ShapeCreator3000::CreatePlane(), ToString(ePrimitiveShape::Plane), *device.Get(), context);
				AddModel(assetManager, ShapeCreator3000::CreateSkyBox(Vector3f(100, 100, 100)), ToString(ePrimitiveShape::SkyBox), *device.Get(), context);
				AddModel(assetManager, ShapeCreator3000::CreateSphere(), ToString(ePrimitiveShape::Sphere), *device.Get(), context);
			};
	}

	RenderContext DX11Foundation::CreateRenderContext(const Dimension2u& size)
	{
		DX11RenderContext dx11RenderContext(GetDevice(), GetContext(), size);

		return RenderContext(std::move(dx11RenderContext));
	}
}