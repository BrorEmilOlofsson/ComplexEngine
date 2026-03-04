#pragma once
#include <string>
#include <memory>

#include "Engine/Win/WinWindow.hpp"
#include "Engine/OperatingSystem/WindowHandle.hpp"
#include "Engine/Graphics/GraphicsFoundation.hpp"
#include "Engine/Utility/Win/WinUtility.hpp"

#ifdef _WIN32

#include "Engine/Win/WinDefines.hpp"
#include <Windows.h>

namespace CLX
{

	using WindowID = unsigned int;

	class Win_OperatingSystem final
	{
	public:

		explicit Win_OperatingSystem(HINSTANCE instanceHandle, std::wstring className);

		Win_OperatingSystem(const Win_OperatingSystem&) = delete;
		Win_OperatingSystem(Win_OperatingSystem&&);
		Win_OperatingSystem& operator=(const Win_OperatingSystem&) = delete;
		Win_OperatingSystem& operator=(Win_OperatingSystem&&) = delete;

		void Init();
		void Shutdown();
		void BeginFrame(const GraphicsBufferData& bufferData);
		void EndFrame(RenderContext* renderContext);

		LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		unsigned int MakeWindow(Vector2ui size, std::wstring title);
		[[nodiscard]] Win_Window& GetWindow(const WindowID windowID);
		[[nodiscard]] const Win_Window& GetWindow(const WindowID windowID) const;


		[[nodiscard]] GraphicsFoundation& GetGraphicsFoundation() noexcept;
		[[nodiscard]] const GraphicsFoundation& GetGraphicsFoundation() const noexcept;

		void LoadCursors(const std::filesystem::path& path);
		void SetCursor(const std::filesystem::path& path);
		void ShowCursor();
		void HideCursor();

		[[nodiscard]] bool IsCursorVisible() const;

	private:

		HINSTANCE mInstanceHandle = HINSTANCE{};
		std::unique_ptr<Win_WindowClass> mWindowClass;
		GraphicsFoundation mGraphicsFoundation;
		std::vector<std::unique_ptr<Win_Window>> mWindows;

		std::unordered_map<std::filesystem::path, HCURSOR> mCursors;
		bool mIsCursorVisible = true;
	};

	inline void OSBeginFrame(Win_OperatingSystem& os, const GraphicsBufferData& data)
	{
		os.BeginFrame(data);
	}

	inline void OSEndFrame(Win_OperatingSystem& os, RenderContext* renderContext)
	{
		os.EndFrame(renderContext);
	}

	inline void OSInit(Win_OperatingSystem& os)
	{
		os.Init();
	}

	inline void OSShutdown(Win_OperatingSystem& os)
	{
		os.Shutdown();
	}

	[[nodiscard]] inline Win_Window& GetOSWindow(Win_OperatingSystem& os, WindowHandle windowHandle)
	{
		return os.GetWindow(windowHandle.id);
	}

	[[nodiscard]] inline const Win_Window& GetOSWindow(const Win_OperatingSystem& os, WindowHandle windowHandle)
	{
		return os.GetWindow(windowHandle.id);
	}

	[[nodiscard]] inline WindowHandle OSCreateWindow(Win_OperatingSystem& os, Vector2ui size, std::wstring title)
	{
		return WindowHandle(os.MakeWindow(size, title));
	}

	[[nodiscard]] inline GraphicsFoundation& OSGetGraphicsFoundation(Win_OperatingSystem& os)
	{
		return os.GetGraphicsFoundation();
	}

	[[nodiscard]] inline const GraphicsFoundation& OSGetGraphicsFoundation(const Win_OperatingSystem& os)
	{
		return os.GetGraphicsFoundation();
    }

	inline void OSLoadCursors(Win_OperatingSystem& os, const std::filesystem::path& path)
	{
		os.LoadCursors(path);
	}

	inline void OSSetCursor(Win_OperatingSystem& os, const std::filesystem::path& path)
	{
		os.SetCursor(path);
	}

	inline void OSShowCursor(Win_OperatingSystem& os)
	{
		os.ShowCursor();
	}

	inline void OSHideCursor(Win_OperatingSystem& os)
	{
		os.HideCursor();
	}

	[[nodiscard]] inline Point2i OSGetCursorPos(const Win_OperatingSystem&)
	{
		return GetCursorPosition();
	}

	[[nodiscard]] inline bool OSIsCursorVisible(const Win_OperatingSystem& os)
	{
		return os.IsCursorVisible();
	}

	[[nodiscard]] inline void* OSGetForegroundWindow(const Win_OperatingSystem&)
	{
		return GetForegroundWindow();
	}
}

#endif