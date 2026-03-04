#include "Engine/Precompiled/EnginePch.hpp"
#include "OperatingSystem.hpp"

namespace CLX
{

	void OperatingSystem::BeginFrame(const GraphicsBufferData& bufferData)
	{
		mConcept->BeginFrame(bufferData);
	}

	void OperatingSystem::EndFrame(RenderContext* renderContext)
	{
		mConcept->EndFrame(renderContext);
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

	GraphicsFoundation& OperatingSystem::GetGraphicsFoundation()
	{
		return mConcept->GetGraphicsFoundation();
	}

	const GraphicsFoundation& OperatingSystem::GetGraphicsFoundation() const
	{
		return mConcept->GetGraphicsFoundation();
    }


    WindowHandle OperatingSystem::MakeWindow(Vector2ui size, std::wstring title)
	{
		return mConcept->MakeWindow(size, title);
	}

	void OperatingSystem::LoadCursors(const std::filesystem::path& path)
	{
		mConcept->LoadCursors(path);
	}

	void* OperatingSystem::GetForegroundWindow() const
	{
		return mConcept->GetForegroundWindow();
    }

	bool OperatingSystem::IsCursorVisible() const
	{
        return mConcept->IsCursorVisible();
	}

	Point2i OperatingSystem::GetCursorScreenPosition() const
	{
		return mConcept->GetCursorScreenPosition();
    }

	void OperatingSystem::ShowCursor()
	{
		mConcept->ShowCursor();
    }

	void OperatingSystem::HideCursor()
	{
		mConcept->HideCursor();
	}
}