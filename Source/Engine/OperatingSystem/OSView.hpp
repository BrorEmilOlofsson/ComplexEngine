#pragma once
#include <filesystem>

namespace Simple
{
	
	class OSView final
	{
	public:

		template<typename T>
		explicit OSView(T& os)
			: mOS(&os)
			, mLoadCursorsFunc([](void* os, const std::filesystem::path& path) { OS_LoadCursors(*static_cast<T*>(os), path); })
			, mSetCursorFunc([](void* os, const std::filesystem::path& path) { OS_SetCursor(*static_cast<T*>(os), path); })
			, mShowCursorFunc([](void* os) { OS_ShowCursor(*static_cast<T*>(os)); })
			, mHideCursorFunc([](void* os) { OS_HideCursor(*static_cast<T*>(os)); })
			, mGetCursorPosFunc([](const void* os) { return OS_GetCursorPos(*static_cast<const T*>(os)); })
			, mIsCursorVisibleFunc([](const void* os) { return OS_IsCursorVisible(*static_cast<const T*>(os)); })
			, mGetForegroundWindowFunc([](const void* os) { return OS_GetForegroundWindow(*static_cast<const T*>(os)); })
		{
		}

		void LoadCursors(const std::filesystem::path& path)
		{
			mLoadCursorsFunc(mOS, path);
		}

		void SetCursor(const std::filesystem::path& path)
		{
			mSetCursorFunc(mOS, path);
		}

		void ShowCursor() const
		{
			mShowCursorFunc(mOS);
		}

		void HideCursor() const
		{
			mHideCursorFunc(mOS);
		}

		[[nodiscard]] Point2i GetCursorPos() const
		{
			return mGetCursorPosFunc(mOS);
		}

		[[nodiscard]] bool IsCursorVisible() const
		{
			return mIsCursorVisibleFunc(mOS);
		}

		[[nodiscard]] void* GetForegroundWindow() const
		{
			return mGetForegroundWindowFunc(mOS);
		}

	private:

		void* mOS = nullptr;
		void (*mLoadCursorsFunc)(void*, const std::filesystem::path&);
		void (*mSetCursorFunc)(void*, const std::filesystem::path&);
		void (*mShowCursorFunc)(void*);
		void (*mHideCursorFunc)(void*);
		Point2i (*mGetCursorPosFunc)(const void*);
		bool (*mIsCursorVisibleFunc)(const void*);
		void* (*mGetForegroundWindowFunc)(const void*) = nullptr;
	};
}