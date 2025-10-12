#include "Engine/Precompiled/EnginePch.hpp"
#include "WinInputProcessor.hpp"
#include <windowsx.h>
#include <string>

namespace Simple
{

	static constexpr eInputKey ToSimpleKey(unsigned int winKey)
	{
		return static_cast<eInputKey>(winKey);
	}

	void Win_InputProcessor::Update()
	{
		mInputState.Update(mMouse.mTentativePosition, mMouse.mTentativePositionDelta, mMouse.mTentativeWheelDelta);
		mMouse.mTentativePositionDelta = Vector2i::Zero();
		mMouse.mTentativeWheelDelta = 0;
	}

	bool Win_InputProcessor::HandleMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Vector2ui windowSize)
	{
		if (mMouse.HandleMessages(hWnd, message, wParam, lParam, windowSize))
		{
			return true;
		}

		switch (message)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			mInputState[ToSimpleKey(static_cast<unsigned int>(wParam))] = true;
			break;
		case WM_LBUTTONDOWN:
			mInputState[ToSimpleKey(VK_LBUTTON)] = true;
			break;
		case WM_RBUTTONDOWN:
			mInputState[ToSimpleKey(VK_RBUTTON)] = true;
			break;
		case WM_MBUTTONDOWN:
			mInputState[ToSimpleKey(VK_MBUTTON)] = true;
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			mInputState[ToSimpleKey(static_cast<unsigned int>(wParam))] = false;
			break;
		case WM_LBUTTONUP:
			mInputState[ToSimpleKey(VK_LBUTTON)] = false;
			break;
		case WM_RBUTTONUP:
			mInputState[ToSimpleKey(VK_RBUTTON)] = false;
			break;
		case WM_MBUTTONUP:
			mInputState[ToSimpleKey(VK_MBUTTON)] = false;
			break;
		default:
			return false;
		}

		return true;
	}

	void Win_InputProcessor::ResetKeyStates()
	{
		mInputState.Reset();
	}
}