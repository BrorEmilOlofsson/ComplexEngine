#include "Engine/Precompiled/EnginePch.hpp"
#include "OperatingSystem.hpp"

namespace CLX
{

	void OperatingSystem::BeginFrame()
	{
		mConcept->BeginFrame();
	}

	void OperatingSystem::EndFrame(RenderContext* renderContext)
	{
		mConcept->EndFrame(renderContext);
	}

	WindowView OperatingSystem::GetWindow(WindowHandle windowHandle)
	{
		return mConcept->GetWindow(windowHandle);
	}

	CWindowView OperatingSystem::GetWindow(WindowHandle windowHandle) const
	{
		return mConcept->GetCWindow(windowHandle);
	}

	WindowHandle OperatingSystem::MakeWindow(Dimension2u size, std::wstring title)
	{
		return mConcept->MakeWindow(size, title);
	}

	//GraphicsFoundation& OperatingSystem::GetGraphicsFoundation()
	//{
	//	return mConcept->GetGraphicsFoundation();
	//}

	//const GraphicsFoundation& OperatingSystem::GetGraphicsFoundation() const
	//{
	//	return mConcept->GetGraphicsFoundation();
 //   }

	const InputState& OperatingSystem::GetInputState() const
	{
        return mConcept->GetInputState();
	}

	void OperatingSystem::LoadCursors(const std::filesystem::path& path)
	{
		mConcept->LoadCursors(path);
	}

	const WindowFrameBuffer& OperatingSystem::GetFrameBuffer() const
	{
		return mConcept->GetFrameBuffer();
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