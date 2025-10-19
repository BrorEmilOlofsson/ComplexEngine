#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11Window.hpp"
#include "Utility/Algorithm.hpp"
#include "Graphics/DX11/DX11Factory.hpp"
#include "Utility/Win/WinException.hpp"
#include "Engine/Win/WinWindow.hpp"
#include "Utility/Win/WinUtility.hpp"
#include "Utility/ShapeMath.hpp"
#include "Graphics/DX11/Texture/DX11Texture.hpp"
#include "Utility/Asset/AssetManager.hpp"
#include "Engine/OperatingSystem/WindowView.hpp"
#include "Graphics/Texture/Texture.hpp"
#include <External/imgui/imgui.h>
#include <External/nlohmann/json.hpp>
#include <fstream>

#ifdef _DEBUG
#define REPORT_DX_WARNINGS
#endif

namespace Simple
{

	void LoadSettingsFromJson(bool& vSync)
	{
		const std::filesystem::path filename = std::filesystem::absolute(SIMPLE_SETTINGS_GAME);

		std::ifstream file(filename);
		if (!file.is_open())
		{
			return;
		}

		const nlohmann::json json = nlohmann::json::parse(file);
		file.close();

		vSync = json["Game_Settings"]["VSync"];
	}

	std::optional<DX11ImGuiWindow> CreateImGuiWindow(const bool instantiate, HWND handle)
	{
		if (instantiate)
		{
			return std::make_optional<DX11ImGuiWindow>(handle);
		}

		return std::nullopt;
	}

	DX11Window::DX11Window(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,
		Win_Window* window, std::shared_ptr<AssetManager> assetManager, std::shared_ptr<GraphicsSettings> graphicsSettings,
		std::weak_ptr<DX11DepthStencilViewManager> dsvManager, bool instantiateImGui)
		: mDevice(device)
		, mContext(context)
		, mSwapChain(DX11Factory::CreateSwapChain(window->GetHandle(), window->GetClientSize(), device))
		, mWindow(window)
		, mAssetManager(assetManager)
		, mGraphicsSettings(graphicsSettings)
		, mBackBuffer(context, device, *DX11Factory::GetBackBuffer(*mSwapChain.Get()).Get(), window->GetClientSize(), false)
		, mImGuiWindow(CreateImGuiWindow(instantiateImGui, window->GetHandle()))
		, mDepthStencilViewManager(dsvManager)
	{
	}

	void DX11Window::Init()
	{
		LoadSettingsFromJson(mGraphicsSettings.lock()->vSync);

		{
			const Vector2ui windowSize = mWindow->GetClientSize();

			const D3D11_VIEWPORT viewport = DX11Factory::CreateViewport(windowSize);

			mContext->RSSetViewports(1, &viewport);

			{
				//auto backBuffer = ;

				//mBackBuffer.InitRenderTargetView(*backBuffer.Get(), windowSize);

				/*{
					mIDTexture = DX11Factory::CreateRenderTargetTexture(*mDevice.Get(), DX11Factory::CreateObjectSelectionTextureDesc(windowSize));
					mObjectIDRenderTarget.Init(*mDevice.Get(), *mIDTexture.Get(), windowSize);
				}*/
				/*{

					D3D11_TEXTURE2D_DESC desc = {};
					mIDTexture->GetDesc(&desc);

					desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
					desc.Usage = D3D11_USAGE_STAGING;
					desc.BindFlags = 0;
					desc.MiscFlags = 0;

					HRESULT hr = mDevice->CreateTexture2D(&desc, nullptr, &mStagingTexture);
					WIN_CHECK_HRESULT(hr);
				}*/
			}

			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState = DX11Factory::CreateDepthBuffer(*mDevice.Get());

			mContext->OMSetDepthStencilState(depthStencilState.Get(), 0);

			mRasterizerState = DX11Factory::CreateRasterizerState_BackfaceCulling(*mDevice.Get());
		}

		mDepthStencilViewHandle = mDepthStencilViewManager.lock()->Create(mWindow->GetClientSize());

		mContext->RSSetState(mRasterizerState.Get());
	}

	void DX11Window::BeginFrame()
	{
		PrepareFrame();
		mBackBuffer.Clear(mGraphicsSettings.lock()->clearColor);
	}

	void DX11Window::EndFrame(std::optional<RenderTargetView> renderTargetView)
	{
		PROFILER_FUNCTION(profiler::colors::Indigo800);

		BindBackBuffer();

		if (renderTargetView)
		{
			RenderFullScreen(
				*mContext.Get(),
				renderTargetView.value(),
				*mSamplerState.lock(),
				mAssetManager.lock()->GetVertexShader(GetPath(eVertexShaderType::FullScreen)),
				mAssetManager.lock()->GetPixelShader(GetPath(ePixelShaderType::FullScreen))
			);
		}

		PROFILER_BEGIN("Present Frame");
		const UINT syncInterval = mGraphicsSettings.lock()->vSync ? 1 : 0;
		const HRESULT result = mSwapChain->Present(syncInterval, 0);
		WIN_CHECK_HRESULT(result);
		PROFILER_END();
	}

	void DX11Window::PrepareFrame()
	{
		/*const float totalTime = static_cast<float>(mTotalTimer.GetDeltaTime().count());
		const float deltaTime = mFrameTimer.GetDeltaTime().count();

		mConstantBufferManager.UpdateTimeBuffer(totalTime, deltaTime, *mContext.Get());
		mConstantBufferManager.UpdatePostProcessBuffer(PostProcessBufferData{}, *mContext.Get());

		mRenderTargetManager.ClearAll(mGraphicsSettings->clearColor);*/
		//mObjectIDRenderTarget.Clear(Colors::White);

		/*for (auto& depthStencilView : mDepthStencilViewManager)
		{
			mContext->ClearDepthStencilView(depthStencilView.second.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}*/
	}

	//static void RenderToObjectIDRenderTarget(ID3D11DeviceContext& context, ID3D11DepthStencilView* dsv,
	//	DX11ConstantBuffer<ObjectIDBufferData>& objectIDCB, DX11ConstantBuffer<ColorBufferData>& colorCB, DX11ConstantBuffer<TransformBufferData>& transformBuffer,
	//	const RenderState& renderState, DX11Renderer& renderer, AssetManager& assetManager,
	//	ID3D11Resource* stagingTexture, ID3D11Resource* idTexture, DX11RenderTarget& objectIDRenderTarget, Point2i cursorPos,
	//	Vector2ui windowSize, const AABB2i& renderRect)
	//{
	//	PROFILER_FUNCTION(profiler::colors::Blue);
	//	context.ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//	objectIDCB.UpdateAndBind(ObjectIDBufferData{ 20000 }, context);
	//	objectIDRenderTarget.Set(*dsv);

	//	renderer.Render(
	//		renderState,
	//		assetManager,
	//		assetManager.GetPixelShader(GetPath(ePixelShaderType::ObjectID)),
	//		assetManager.GetVertexShader(GetPath(eVertexShaderType::Default)),
	//		colorCB,
	//		transformBuffer,
	//		renderTar
	//	);

	//	const AABB2f windowRect = AABB2f::CreateFromMinAndExtent(Point2f::Zero(), Vector2f(windowSize));
	//	const Point2i mappedPos = Point2i(Remap(static_cast<Point2f>(cursorPos), ToAABB<float>(renderRect), windowRect));

	//	if (IsInside(mappedPos, ToAABB<int>(windowRect)))
	//	{
	//		PROFILER_BEGIN("Copy Resource");
	//		// Copy GPU texture to a staging resource
	//		context.CopyResource(stagingTexture, idTexture);
	//		PROFILER_END();

	//		PROFILER_BEGIN("Map Resource");
	//		D3D11_MAPPED_SUBRESOURCE mapped;
	//		context.Map(stagingTexture, 0, D3D11_MAP_READ, 0, &mapped);

	//		struct ColorUINT_8 final
	//		{
	//			uint8_t r = 0;
	//			uint8_t g = 0;
	//			uint8_t b = 0;
	//			uint8_t a = 0;
	//		};

	//		const ColorUINT_8* pixelData = reinterpret_cast<const ColorUINT_8*>(mapped.pData);

	//		const int pitch = mapped.RowPitch / sizeof(ColorUINT_8); // RowPitch is in bytes
	//		const int index = static_cast<int>(mappedPos.y * pitch) + static_cast<int>(mappedPos.x);
	//		ColorUINT_8 pixel = pixelData[index];

	//		// Reconstruct ID
	//		const uint32_t id = pixel.r | (pixel.g << 8) | (pixel.b << 16) | (pixel.a << 24);
	//		id;

	//		context.Unmap(stagingTexture, 0);
	//		PROFILER_END();
	//	}
	//}

	static void ValidateRenderState(const RenderState& renderState)
	{
		if (!renderState.GetCamera())
		{
			throw std::runtime_error("No camera set in RenderState");
		}

		if (!renderState.GetRenderTargetView())
		{
			throw std::exception("No render target set in RenderState");
		}

		if (!renderState.GetDepthStencilViewHandle())
		{
			throw std::exception("No depth stencil view set in RenderState");
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

	//void DX11Window::Render(const RenderState& renderState)
	//{
	//	PROFILER_FUNCTION(profiler::colors::Red);

	//	ValidateRenderState(renderState);

	//	mRenderTargetManager.Get(*renderState.GetRenderTargetView())->Set(*mDepthStencilViewManager.Get(renderState.GetDepthStencilViewHandle().value()));
	//	mAssetManager->GetTexture(GetPath(eIconType::Scene))->Bind();
	//	mSamplerState.Bind(*mContext.Get());

	//	mConstantBufferManager.UpdatePointLights(CreatePointLightData(renderState.GetRenderList().GetPointLights()), *mContext.Get());

	//	mConstantBufferManager.UpdateCameraBuffer(*renderState.GetCamera(), mWindow->GetClientSize(), *mContext.Get());
	//	mRenderer.Render(
	//		renderState,
	//		*mAssetManager,
	//		mAssetManager->GetPixelShader(GetPath(ePixelShaderType::LitDefault)),
	//		mAssetManager->GetVertexShader(GetPath(eVertexShaderType::Default)),
	//		mWindow->GetClientSize(),
	//		mConstantBufferManager.mColorBuffer
	//	);

	//	/*POINT mousePos;
	//	GetCursorPos(&mousePos);

	//	RenderToObjectIDRenderTarget(myContext.Get(),
	//		myDepthStencilView.Get(), myConstantBufferManager->myObjectIDBuffer,
	//		renderList, *myRenderer, *myCurrentCamera,	*myShaderManager,
	//		myStagingTexture.Get(), myIDTexture.Get(), myObjectIDRenderTarget,
	//		ToPoint(mousePos), myWindow->GetClientSize(), myRenderAABB);*/

	//}

	void DX11Window::OnWindowResize()
	{
		const Vector2ui windowSize = mWindow->GetClientSize();

		/*for (auto& depthStencilView : mDepthStencilViewManager)
		{
			depthStencilView.second->Release();
		}*/
		mBackBuffer.GetRenderTargetView()->Release();

		const HRESULT result = mSwapChain->ResizeBuffers(0, windowSize.x, windowSize.y, DXGI_FORMAT_UNKNOWN, 0);
		WIN_CHECK_HRESULT(result);

		auto backBuffer = DX11Factory::GetBackBuffer(*mSwapChain.Get());
		mBackBuffer.InitRenderTargetView(*backBuffer.Get(), windowSize);

		//mDepthStencilViewManager.InitializeAll(windowSize);

		// Initialize depth stencil view

		/*auto texture = DX11Factory::CreateRenderTargetTexture(*mDevice.Get(), DX11Factory::CreateRenderTargetTextureDesc(windowSize));

		for (auto& renderTarget : mRenderTargetManager)
		{
			renderTarget.second.Init(*mDevice.Get(), *texture.Get());
		}*/

		{
			auto viewport = DX11Factory::CreateViewport(windowSize);
			mContext->RSSetViewports(1, &viewport);
		}
	}

	void DX11Window::BindBackBuffer()
	{
		mBackBuffer.Set(*mDepthStencilViewManager.lock()->Get(mDepthStencilViewHandle).Get());
	}
}
