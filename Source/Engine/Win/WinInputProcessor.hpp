#pragma once
#ifdef _WIN32

#include "Engine/Win/WinDefines.hpp"
#include <Windows.h>

#include "Engine/Math/Dimension2.hpp"
#include "Engine/Win/WinMouseInput.hpp"
#include "Engine/Input/InputState.hpp"


namespace CLX
{
	class Win_InputProcessor final
	{
	public:

		Win_InputProcessor() = default;

		
		bool HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void Update();

		void ResetKeyStates();

		const InputState& GetInputState() const
		{
			return mInputState;
		}

	private:

		Win_MouseInput mMouse;
		InputState mInputState;
		InputKeyState mLiveKeyState;
	};
}

#endif