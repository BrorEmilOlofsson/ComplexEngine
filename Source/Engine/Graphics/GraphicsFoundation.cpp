#include "Engine/Precompiled/EnginePch.hpp"
#include "GraphicsFoundation.hpp"

namespace CLX
{

    void GraphicsFoundation::BeginFrame(const GraphicsBufferData& bufferData)
    {
        mConcept->BeginFrame(bufferData);
    }

    void GraphicsFoundation::EndFrame()
    {
        mConcept->EndFrame();
    }

    void GraphicsFoundation::Init()
    {
        mConcept->Init();
    }

    void GraphicsFoundation::Shutdown()
    {
        mConcept->Shutdown();
    }

    void GraphicsFoundation::Render(RenderState& renderState)
    {
        mConcept->Render(renderState);
    }

    RenderContext GraphicsFoundation::CreateRenderContext(const Dimension2u& size)
    {
        return mConcept->CreateRenderContext(size);
    }

    GraphicsWindowView GraphicsFoundation::MakeWindow(WindowView windowView)
    {
        return mConcept->MakeWindow(windowView);
    }

    void GraphicsFoundation::SetAssetManager(std::weak_ptr<AssetManager> assetManager)
    {
        mConcept->SetAssetManager(std::move(assetManager));
    }

    void GraphicsFoundation::SetGraphicsSettings(std::weak_ptr<GraphicsSettings> graphicsSettings)
    {
        mConcept->SetGraphicsSettings(std::move(graphicsSettings));
    }
}