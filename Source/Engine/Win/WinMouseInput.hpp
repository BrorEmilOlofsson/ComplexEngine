#pragma once
#include "Utility/Math/Point2.hpp"
#include "Utility/Math/Vector2.hpp"
#include "Utility/Input/InputMouseState.hpp"

#ifdef _WIN32

#include "Engine/Win/WinDefines.hpp"
#include <Windows.h>

namespace Simple
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