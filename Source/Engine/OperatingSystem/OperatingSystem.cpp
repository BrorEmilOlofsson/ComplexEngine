#include "Engine/Precompiled/EnginePch.hpp"
#include "OperatingSystem.hpp"

namespace Simple
{

	void OperatingSystem::BeginFrame(const GraphicsBufferData& bufferData)
	{
		mConcept->BeginFrame(bufferData);
	}

	void OperatingSystem::EndFrame(RenderContext* renderContext)
	{
		mConcept->EndFrame(renderContext);
	}

	void OperatingSystem::Render()
	{
		mConcept->Render();
	}

	void OperatingSystem::Render(RenderState& renderState)
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

	GraphicsFoundation& OperatingSystem::GetGraphicsFoundation()
	{
		return mConcept->GetGraphicsFoundation();
	}

	const GraphicsFoundation& OperatingSystem::GetGraphicsFoundation() const
	{
		return mConcept->GetGraphicsFoundation();
    }

	void OperatingSystem::LoadCursors(const std::filesystem::path& path)
	{
		GetOS().LoadCursors(path);
	}
}