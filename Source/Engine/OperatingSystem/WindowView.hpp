#pragma once
#include "WindowFrameBuffer.hpp"
#include "Engine/Math/Shapes/AABB2.hpp"
#include "Engine/Input/InputState.hpp"
#include "Engine/Utility/SystemTimer.hpp"

namespace CLX
{

	struct WindowOperations final
	{
		template<typename T>
		explicit WindowOperations(std::type_identity<T>)
			: showFunc([](void* window) { return ShowWindow(*static_cast<T*>(window)); })
			, hideFunc([](void* window) { return HideWindow(*static_cast<T*>(window)); })
			, captureCursorFunc([](void* window) -> void { return Window_CaptureCursor(*static_cast<T*>(window)); })
			, releaseCursorFunc([](void* window) -> void { return Window_ReleaseCursor(*static_cast<T*>(window)); })
			, setSizeFunc([](void* window, Dimension2u size, const bool fullScreen) -> void { return Window_SetSize(*static_cast<T*>(window), size, fullScreen); })
			, toggleFullScreen([](void* window) -> void { return Window_ToggleFullScreen(*static_cast<T*>(window)); })
		{
		}

		void (*showFunc)(void*) = nullptr;
		void (*hideFunc)(void*) = nullptr;
		void (*captureCursorFunc)(void*) = nullptr;
		void (*releaseCursorFunc)(void*) = nullptr;
		void (*setSizeFunc)(void*, const Dimension2u, const bool) = nullptr;
		void (*toggleFullScreen)(void*) = nullptr;
	};

	struct WindowConstOperations final
	{
		template<typename T>
		explicit WindowConstOperations(std::type_identity<T>)
			: getFrameBufferFunc([](const void* window) -> const WindowFrameBuffer& { return GetWindowFrameBuffer(*static_cast<const T*>(window)); })
			, getBoundsFunc([](const void* window) { return GetWindowBounds(*static_cast<const T*>(window)); })
			, getClientBoundsFunc([](const void* window) { return GetClientWindowBounds(*static_cast<const T*>(window)); })
			, getInputStateFunc([](const void* window) -> const InputState& { return GetWindowInputState(*static_cast<const T*>(window)); })
			, getHandleFunc([](const void* window) { return GetWindowHandle(*static_cast<const T*>(window)); })
		{
		}

		AABB2i(*getBoundsFunc)(const void*) = nullptr;
		AABB2i(*getClientBoundsFunc)(const void*) = nullptr;
		const WindowFrameBuffer& (*getFrameBufferFunc)(const void*) = nullptr;
		const InputState& (*getInputStateFunc)(const void*) = nullptr;
		void* (*getHandleFunc)(const void*) = nullptr;
	};

	class WindowView final
	{
	public:

		template<typename T>
		explicit WindowView(T& window)
			: mWindow(std::addressof(window))
			, mOperations(std::type_identity<T>{})
			, mConstOperations(std::type_identity<T>{})
		{
		}

		void Show() const
		{
			mOperations.showFunc(mWindow);
		}

		void Hide() const
		{
			mOperations.hideFunc(mWindow);
		}

		void SetSize(Dimension2u size, const bool fullScreen) const
		{
			mOperations.setSizeFunc(mWindow, size, fullScreen);
		}

		void ToggleFullScreen() const
		{
			mOperations.toggleFullScreen(mWindow);
		}

		void CaptureCursor() const noexcept
		{
			return mOperations.captureCursorFunc(mWindow);
		}

		void ReleaseCursor() const noexcept
		{
			return mOperations.releaseCursorFunc(mWindow);
		}

		[[nodiscard]] const WindowFrameBuffer& GetFrameBuffer() const
		{
			return mConstOperations.getFrameBufferFunc(mWindow);
		}

		[[nodiscard]] Dimension2u GetClientSize() const noexcept
		{
			return ToDimension2(GetClientBounds().GetExtent());
		}

		[[nodiscard]] float GetAspectRatio() const noexcept
		{
			return ToAspectRatio(GetClientSize());
		}

		[[nodiscard]] AABB2i GetBounds() const noexcept
		{
			return mConstOperations.getBoundsFunc(mWindow);
		}

		[[nodiscard]] AABB2i GetClientBounds() const noexcept
		{
			return mConstOperations.getClientBoundsFunc(mWindow);
		}

		[[nodiscard]] const InputState& GetInputState() const noexcept
		{
			return mConstOperations.getInputStateFunc(mWindow);
		}

		[[nodiscard]] void* GetHandle() const
		{
			return mConstOperations.getHandleFunc(mWindow);
		}

	private:

		void* mWindow = nullptr;
		WindowOperations mOperations;
		WindowConstOperations mConstOperations;
	};


	class CWindowView final
	{
	public:

		template<typename T>
		explicit CWindowView(const T& window)
			: mWindow(&window)
			, mOperations(std::type_identity<T>{})
		{
		}

		const WindowFrameBuffer& GetFrameBuffer() const
		{
			return mOperations.getFrameBufferFunc(mWindow);
		}

		[[nodiscard]] Dimension2u GetClientSize() const noexcept
		{
			return ToDimension2(GetClientBounds().GetExtent());
		}

		[[nodiscard]] AABB2i GetBounds() const noexcept
		{
			return mOperations.getBoundsFunc(mWindow);
		}

		[[nodiscard]] AABB2i GetClientBounds() const noexcept
		{
			return mOperations.getClientBoundsFunc(mWindow);
		}

		[[nodiscard]] const InputState& GetInputState() const noexcept
		{
			return mOperations.getInputStateFunc(mWindow);
		}

		[[nodiscard]] void* GetHandle() const
		{
			return mOperations.getHandleFunc(mWindow);
		}

	private:

		const void* mWindow = nullptr;
		WindowConstOperations mOperations;
	};
}