#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11Window.hpp"
#include "Engine/Graphics/DX11/DX11Factory.hpp"
#include "Engine/Utility/Win/WinException.hpp"
#include "Engine/Asset/AssetManager.hpp"
#include "Engine/OperatingSystem/WindowView.hpp"
#include "Engine/Graphics/DX11/RenderTarget/DX11RenderTarget.hpp"
#include "Engine/Graphics/DX11/DX11RenderContext.hpp"
#include "Engine/Graphics/DX11/DX11RenderFunctions.hpp"
#include "Engine/Win/WinOperatingSystem.hpp"

#ifdef _DEBUG
#define REPORT_DX_WARNINGS
#endif

namespace CLX
{

    std::optional<DX11ImGuiWindow> CreateImGuiWindow(const bool instantiate, HWND handle)
    {
        if (instantiate)
        {
            return std::make_optional<DX11ImGuiWindow>(handle);
        }

        return std::nullopt;
    }

    DX11Window::DX11Window(Microsoft::WRL::ComPtr<ID3D11Device> device,
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain,
        Win_OperatingSystem* operatingSystem, WindowHandle windowHandle,
        std::weak_ptr<AssetManager> assetManager, std::weak_ptr<GraphicsSettings> graphicsSettings,
        DX11RenderTarget backBuffer, std::optional<DX11ImGuiWindow> imGuiWindow,
        std::weak_ptr<DX11DepthStencilViewManager> dsvManager,
        std::weak_ptr<DX11SamplerState> samplerState)
        : mDevice(device)
        , mContext(context)
        , mSwapChain(swapChain)
        , mOperatingSystem(operatingSystem)
        , mWindowHandle(windowHandle)
        //, mWindowView(std::move(windowView))
        , mAssetManager(assetManager)
        , mGraphicsSettings(graphicsSettings)
        , mBackBuffer(std::move(backBuffer))
        , mImGuiWindow(std::move(imGuiWindow))
        , mDepthStencilViewManager(dsvManager)
        , mSamplerState(std::move(samplerState))
    {}

    DX11Window DX11Window::Create(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,
        Win_OperatingSystem* operatingSystem, WindowHandle windowHandle, std::weak_ptr<AssetManager> assetManager, std::weak_ptr<GraphicsSettings> graphicsSettings,
        std::weak_ptr<DX11DepthStencilViewManager> dsvManager, std::weak_ptr<DX11SamplerState> samplerState, bool instantiateImGui)
    {
        Win_Window& window = operatingSystem->GetWindow(windowHandle);
        HWND hwnd = window.GetHandle();
        Dimension2u windowSize = window.GetClientSize();
        Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain = DX11Factory::CreateSwapChain(hwnd, windowSize, device);
        std::optional<DX11ImGuiWindow> imGuiWindow = CreateImGuiWindow(instantiateImGui, hwnd);
        DX11RenderTarget backBuffer(context, device, *DX11Factory::GetBackBuffer(*swapChain.Get()).Get(), windowSize, false);
        return DX11Window(
            device,
            context,
            swapChain,
            operatingSystem,
            windowHandle,
            assetManager,
            graphicsSettings,
            std::move(backBuffer),
            std::move(imGuiWindow),
            dsvManager,
            samplerState
        );
    }

    void DX11Window::Init()
    {
        const Win_Window& window = mOperatingSystem->GetWindow(mWindowHandle);
        {
            const Dimension2u windowSize = window.GetClientSize();

            const D3D11_VIEWPORT viewport = DX11Factory::CreateViewport(windowSize);

            mContext->RSSetViewports(1, &viewport);

            mRasterizerState = DX11Factory::CreateRasterizerState_BackfaceCulling(*mDevice.Get());
        }

        mDepthStencilViewHandle = mDepthStencilViewManager.lock()->Create(window.GetClientSize());

        mContext->RSSetState(mRasterizerState.Get());
    }

    void DX11Window::BeginFrame()
    {
        PrepareFrame();
        mBackBuffer.Clear(mGraphicsSettings.lock()->clearColor);
    }

    void DX11Window::EndFrame(RenderContext* renderContext)
    {
        PROFILER_FUNCTION(profiler::colors::Indigo800);

        BindBackBuffer();

        if (renderContext)
        {
            DX11RenderContext* dx11RenderContext = renderContext->Cast<DX11RenderContext>();
            ASSERT(dx11RenderContext != nullptr, "Failed to cast RenderContext to DX11RenderContext");

            RenderFullScreen(
                *mContext.Get(),
                dx11RenderContext->GetOutputRenderTarget().GetShaderResourceView(),
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
    {}

    void DX11Window::OnResize()
    {
        const Win_Window& window = mOperatingSystem->GetWindow(mWindowHandle);
        const Dimension2u windowSize = window.GetClientSize();

        if (windowSize.GetWidth() == 0 || windowSize.GetHeight() == 0)
        {
            return;
        }

        mContext->OMSetRenderTargets(0, nullptr, nullptr);

        // Release old references safely
        mBackBuffer.Reset();
        mDepthStencilViewManager.lock()->Get(mDepthStencilViewHandle).Reset();

        // Resize swap chain buffers
        HRESULT hr = mSwapChain->ResizeBuffers(0, windowSize.GetWidth(), windowSize.GetHeight(), DXGI_FORMAT_UNKNOWN, 0);
        WIN_CHECK_HRESULT(hr);

        // Create RTV
        auto backBuffer = DX11Factory::GetBackBuffer(*mSwapChain.Get());
        mBackBuffer.InitRenderTargetView(*backBuffer.Get(), windowSize);

        mDepthStencilViewManager.lock()->Initialize(mDepthStencilViewHandle, windowSize);
    }

    void DX11Window::BindBackBuffer()
    {
        const Win_Window& window = mOperatingSystem->GetWindow(mWindowHandle);
        mBackBuffer.Set(mDepthStencilViewManager.lock()->Get(mDepthStencilViewHandle).Get());

        auto viewport = DX11Factory::CreateViewport(window.GetClientSize());

        mContext->RSSetViewports(1, &viewport);
    }
}
