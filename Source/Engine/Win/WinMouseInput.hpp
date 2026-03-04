#pragma once
#include "Engine/Math/Point2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Input/InputMouseState.hpp"

#ifdef _WIN32

#include "Engine/Win/WinDefines.hpp"
#include <Windows.h>

namespace CLX
{

	class Win_MouseInput final
	{
	public:

		Win_MouseInput() = default;

		[[nodiscard]] bool HandleMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Vector2ui aWindowSize);
		
		
		Point2i mTentativePosition;
		Vector2i mTentativePositionDelta;
		int mTentativeWheelDelta = 0;
	};
}

#endif