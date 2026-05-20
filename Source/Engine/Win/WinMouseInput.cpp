#include "Engine/Precompiled/EnginePch.hpp"

#ifdef _WIN32

#define NOMINMAX

#include "WinMouseInput.hpp"
#include <Windows.h>
#include <windowsx.h>

namespace CLX
{

	bool Win_MouseInput::HandleMessage([[maybe_unused]] HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		/*case WM_MOUSEMOVE:
		{
            mTentativePosition = HandleMouseMove(lParam, windowSize);
		}
		break;*/
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

#endif