#include "Engine/Precompiled/EnginePch.hpp"

#ifdef _WIN32

#include "WinInputProcessor.hpp"
#include <Xinput.h>

namespace CLX
{

    HMODULE xinputModule = LoadLibrary(L"xinput1_4.dll");
    decltype(&XInputGetState) getStateFunc = nullptr;
    decltype(&XInputSetState) setStateFunc = nullptr;

    void VibrateController(DWORD userIndex)
    {
        XINPUT_VIBRATION vibration
        {
            .wLeftMotorSpeed = 65'535,
            .wRightMotorSpeed = 0
        };
        setStateFunc(userIndex, &vibration);
    }

    std::array<InputGamepadState, 4> HandleXInputMessages()
    {
        if (xinputModule != nullptr)
        {
            getStateFunc = reinterpret_cast<decltype(&XInputGetState)>(GetProcAddress(xinputModule, "XInputGetState"));
            if (getStateFunc == nullptr)
            {
                ASSERT(false);
            }

            setStateFunc = reinterpret_cast<decltype(&XInputSetState)>(GetProcAddress(xinputModule, "XInputSetState"));
            if (setStateFunc == nullptr)
            {
                ASSERT(false);
            }
        }
        static_assert(XUSER_MAX_COUNT == 4, "XUSER_MAX_COUNT is expected to be 4");
        std::array<InputGamepadState, 4> gamepadStates;
        for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
        {

            XINPUT_STATE state;
            if (getStateFunc(i, &state) == ERROR_SUCCESS)
            {
                XINPUT_GAMEPAD& gamepad = state.Gamepad;
                gamepad;
                bool up = (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
                bool down = (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
                bool left = (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
                bool right = (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
                bool start = (gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
                bool back = (gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
                bool leftThumb = (gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
                bool rightThumb = (gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
                bool leftShoulder = (gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
                bool rightShoulder = (gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
                bool a = (gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
                bool b = (gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
                bool x = (gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
                bool y = (gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
                uint8_t leftTrigger = gamepad.bLeftTrigger;
                uint8_t rightTrigger = gamepad.bRightTrigger;
                int16_t thumbLX = gamepad.sThumbLX;
                int16_t thumbLY = gamepad.sThumbLY;
                int16_t thumbRX = gamepad.sThumbRX;
                int16_t thumbRY = gamepad.sThumbLX;

                gamepadStates[i][eGamepadButton::DPadUp] = up;
                gamepadStates[i][eGamepadButton::DPadDown] = down;
                gamepadStates[i][eGamepadButton::DPadLeft] = left;
                gamepadStates[i][eGamepadButton::DPadRight] = right;
                gamepadStates[i][eGamepadButton::Start] = start;
                gamepadStates[i][eGamepadButton::Back] = back;
                gamepadStates[i][eGamepadButton::LeftThumb] = leftThumb;
                gamepadStates[i][eGamepadButton::RightThumb] = rightThumb;
                gamepadStates[i][eGamepadButton::LeftShoulder] = leftShoulder;
                gamepadStates[i][eGamepadButton::RightShoulder] = rightShoulder;
                gamepadStates[i][eGamepadButton::A] = a;
                gamepadStates[i][eGamepadButton::B] = b;
                gamepadStates[i][eGamepadButton::X] = x;
                gamepadStates[i][eGamepadButton::Y] = y;
                gamepadStates[i].mLeftThumb = { thumbLX, thumbLY };
                gamepadStates[i].mRightThumb= { thumbRX, thumbRY };
                gamepadStates[i].mLeftTriggerValue = leftTrigger;
                gamepadStates[i].mRightTriggerValue = rightTrigger;
            }
        }

        return gamepadStates;
    }

    static constexpr eInputKey ToSimpleKey(unsigned int winKey)
    {
        return static_cast<eInputKey>(winKey);
    }

    void Win_InputProcessor::Update()
    {
        auto gamepadState = HandleXInputMessages();
        mInputState.Update(/*mMouse.mTentativePosition*/{}, mMouse.mTentativePositionDelta, mMouse.mTentativeWheelDelta, mLiveKeyState, gamepadState);
        mMouse.mTentativePositionDelta = Vector2i::Zero();
        mMouse.mTentativeWheelDelta = 0;
    }

    bool Win_InputProcessor::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (mMouse.HandleMessage(hWnd, message, wParam, lParam))
        {
            return true;
        }

        switch (message)
        {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            mLiveKeyState[ToSimpleKey(static_cast<unsigned int>(wParam))] = true;
            break;
        case WM_LBUTTONDOWN:
            mLiveKeyState[ToSimpleKey(VK_LBUTTON)] = true;
            break;
        case WM_RBUTTONDOWN:
            mLiveKeyState[ToSimpleKey(VK_RBUTTON)] = true;
            break;
        case WM_MBUTTONDOWN:
            mLiveKeyState[ToSimpleKey(VK_MBUTTON)] = true;
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            mLiveKeyState[ToSimpleKey(static_cast<unsigned int>(wParam))] = false;
            break;
        case WM_LBUTTONUP:
            mLiveKeyState[ToSimpleKey(VK_LBUTTON)] = false;
            break;
        case WM_RBUTTONUP:
            mLiveKeyState[ToSimpleKey(VK_RBUTTON)] = false;
            break;
        case WM_MBUTTONUP:
            mLiveKeyState[ToSimpleKey(VK_MBUTTON)] = false;
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

#endif