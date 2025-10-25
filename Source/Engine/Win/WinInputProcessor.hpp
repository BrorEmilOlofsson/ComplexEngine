#pragma once
#include "Utility/Math/Vector2.hpp"
#include "Engine/Win/WinMouseInput.hpp"
#include "Utility/Input/InputState.hpp"
#include <bitset>
#include <string>

#ifdef _WIN32

#include "Engine/Win/WinDefines.hpp"
#include <Windows.h>

namespace Simple
{
	class Win_InputProcessor final
	{
	public:

		Win_InputProcessor() = default;

		
		bool HandleMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Vector2ui windowSize);
		void Update();

		void ResetKeyStates();

		const InputState& GetInputState() const
		{
			return mInputState;
		}

	private:

		Win_MouseInput mMouse;
		InputState mInputState;
	};
}

#endif