#include "Engine/Precompiled/EnginePch.hpp"
#include "OperatingSystem.hpp"

namespace Simple
{

	void OperatingSystem::BeginFrame(const GraphicsBufferData& bufferData)
	{
		mConcept->BeginFrame(bufferData);
	}

	void OperatingSystem::EndFrame(std::optional<RenderTargetView> renderTarget)
	{
		mConcept->EndFrame(renderTarget);
	}

	void OperatingSystem::Render()
	{
		mConcept->Render();
	}

	void OperatingSystem::Render(const RenderState& renderState)
	{
		mConcept->Render(renderState);
	}

	void OperatingSystem::Init()
	{
		mConcept->Init();
	}

	void OperatingSystem::Shutdown()
	{
		mConcept->Shutdown();
	}

	WindowView OperatingSystem::GetWindow(WindowHandle windowHandle)
	{
		return mConcept->GetWindow(windowHandle);
	}

	CWindowView OperatingSystem::GetWindow(WindowHandle windowHandle) const
	{
		return mConcept->GetCWindow(windowHandle);
	}

	OSView OperatingSystem::GetOS()
	{
		return mConcept->GetOS();
	}

	RenderTargetView OperatingSystem::CreateRenderTarget(const Vector2ui& size)
	{
		return mConcept->CreateRenderTarget(size);
	}

	DepthStencilViewHandle OperatingSystem::CreateDepthStencilView(const Vector2ui& size)
	{
		return mConcept->CreateDepthStencilView(size);
	}

	void OperatingSystem::LoadCursors(const std::filesystem::path& path)
	{
		GetOS().LoadCursors(path);
	}
}