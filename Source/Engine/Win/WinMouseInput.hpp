#pragma once
#ifdef _WIN32

#define NOMINMAX

#include "Engine/Win/WinDefines.hpp"
#include <Windows.h>

#include "Engine/Math/Point2.hpp"
#include "Engine/Math/Vector2.hpp"


namespace CLX
{

	class Win_MouseInput final
	{
	public:

		Win_MouseInput() = default;

		[[nodiscard]] bool HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		
		
		//Point2i mTentativePosition;
		Vector2i mTentativePositionDelta;
		int mTentativeWheelDelta = 0;
	};
}

#endif