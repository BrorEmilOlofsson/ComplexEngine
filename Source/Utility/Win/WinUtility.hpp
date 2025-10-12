#pragma once
#include "Utility/Math/Vector2.hpp"
#include "Utility/Shapes/AABB2.hpp"

#ifdef _WIN32

#include <Windows.h>
#include "Utility/Win/WinAdapterFunctions.hpp"

namespace Simple
{

	[[nodiscard]] inline AABB2i GetDefaultClientRect(const Vector2ui windowSize)
	{
		AABB2i rect;
		rect.SetMinAndExtent(Point2i(100, 100), static_cast<Vector2i>(windowSize));
		return rect;
	}

	[[nodiscard]] inline AABB2i GetAdjustedWindowRect(const AABB2i& rect, DWORD style)
	{
		RECT winRect = ToRECT(rect);
		AdjustWindowRect(&winRect, style, false);
		return ToAABB<int>(winRect);
	}

	[[nodiscard]] inline AABB2i GetWindowRect(HWND hWND)
	{
		RECT rect{};
		GetWindowRect(hWND, &rect);

		return ToAABB<int>(rect);
	}

	[[nodiscard]] inline Vector2i GetWindowSize(HWND hWND)
	{
		return GetWindowRect(hWND).GetExtent();
	}

	[[nodiscard]] inline AABB2i GetClientRect(HWND hWND)
	{
		RECT rect{};
		GetClientRect(hWND, &rect);

		return ToAABB<int>(rect);
	}

	[[nodiscard]] inline Vector2ui GetClientWindowSize(HWND hWND)
	{
		return Vector2ui(GetClientRect(hWND).GetExtent());
	}

	[[nodiscard]] inline Point2i GetClientToScreenPos(HWND hWND, Point2i aClientPos)
	{
		POINT p = ToPOINT(aClientPos);
		ClientToScreen(hWND, &p);
		return ToPoint(p);
	}

	[[nodiscard]] inline AABB2i GetClientRectInScreenSpace(HWND hWND)
	{
		const AABB2i aabb = GetClientRect(hWND);

		const Point2i min = GetClientToScreenPos(hWND, aabb.GetMin());
		const Point2i max = GetClientToScreenPos(hWND, aabb.GetMax());

		return AABB2i::CreateFromMinAndMax(min, max);
	}

	[[nodiscard]] inline DWORD GetWindowStyleCustom(HWND hWND)
	{
		return static_cast<DWORD>(GetWindowLongPtr(hWND, GWL_STYLE));
	}

	inline void SetWindowStyle(HWND hWND, DWORD style)
	{
		SetWindowLongPtr(hWND, GWL_STYLE, static_cast<LONG_PTR>(style));
	}

	inline void SetWindowRect(HWND hWND, const AABB2i& rect, unsigned int flags)
	{
		const int x = rect.GetMin().x;
		const int y = rect.GetMin().y;
		const unsigned int width = rect.GetExtent().x;
		const unsigned int height = rect.GetExtent().y;
		SetWindowPos(hWND, nullptr, x, y, width, height, flags);
	}

	inline void SetCursorPosition(Point2i position)
	{
		SetCursorPos(position.x, position.y);
	}

	[[nodiscard]] inline Point2i GetCursorPosition()
	{
		POINT p;
		GetCursorPos(&p);
		return ToPoint(p);
	}

	[[nodiscard]] inline AABB2i GetMonitorRect()
	{
		HMONITOR hMonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);

		MONITORINFOEX monitorInfo = { sizeof(MONITORINFOEX) };
		GetMonitorInfo(hMonitor, &monitorInfo);

		return ToAABB<int>(monitorInfo.rcMonitor);
	}

	inline void SetWindowFullscreen(HWND hWND)
	{
		AABB2i rect = GetMonitorRect();
		DWORD style = GetWindowStyleCustom(hWND);
		style &= ~WS_OVERLAPPEDWINDOW;
		style |= WS_POPUP;

		SetWindowStyle(hWND, style);
		SetWindowRect(hWND, rect, SWP_NOZORDER | SWP_FRAMECHANGED);
	}

}

#endif