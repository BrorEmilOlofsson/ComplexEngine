#include "Engine/Precompiled/EnginePch.hpp"
#include "WinMouseInput.hpp"
#include <Windows.h>
#include <windowsx.h>
#include "Engine/Utility/Win/WinUtility.hpp"

namespace Simple
{

	bool Win_MouseInput::HandleMessages([[maybe_unused]] HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Vector2ui windowSize)
	{
		switch (message)
		{
		case WM_MOUSEMOVE:
		{
			const int xPos = GET_X_LPARAM(lParam);
			const int yPos = windowSize.y - GET_Y_LPARAM(lParam);

			mTentativePosition.x = xPos;
			mTentativePosition.y = yPos;
		}
		break;
		case WM_MOUSEWHEEL:
			mTentativeWheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			break;
		case WM_INPUT:
		{
			UINT dwSize = sizeof(RAWINPUT);
			static BYTE lpb[sizeof(RAWINPUT)];

			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

			RAWINPUT* raw = (RAWINPUT*)lpb;

			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				mTentativePositionDelta.x += raw->data.mouse.lLastX;
				mTentativePositionDelta.y -= raw->data.mouse.lLastY;
			}
		}
		break;
		default:
			return false;
			break;
		}

		return true;
	}
}
