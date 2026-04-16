#pragma once
#include "Engine/Input/InputKey.hpp"
#include "Engine/Input/InputMouseState.hpp"
#include <cstddef>
#include <bitset>
#include <array>

namespace CLX
{


    class InputKeyState final
    {
        static constexpr std::size_t InputKeyCount = 255;

    public:

        void Reset()
        {
            mKeyState.reset();
        }

        [[nodiscard]] constexpr bool IsAnyPressed() const
        {
            return mKeyState.any();
        }

        [[nodiscard]] constexpr decltype(auto) operator[](const eInputKey key) noexcept
        {
            return mKeyState[static_cast<std::size_t>(key)];
        }
        [[nodiscard]] constexpr decltype(auto) operator[](const eInputKey key) const noexcept
        {
            return mKeyState[static_cast<std::size_t>(key)];
        }

    private:

        std::bitset<InputKeyCount> mKeyState = {};
    };

    class InputKeyFrameState final
    {
        static constexpr std::size_t InputKeyCount = 255;
    public:

        InputKeyFrameState() = default;

        void Update(const InputKeyState& newKeyState)
        {
            mPreviousKeyState = mKeyState;
            mKeyState = newKeyState;
        }

        void Reset()
        {
            mPreviousKeyState.Reset();
            mKeyState.Reset();
        }

        [[nodiscard]] constexpr bool IsAnyPressed() const
        {
            return mKeyState.IsAnyPressed();
        }

        [[nodiscard]] constexpr bool IsKeyPressed(const eInputKey key) const noexcept
        {
            return mKeyState[key] && !mPreviousKeyState[key];
        }

        [[nodiscard]] constexpr bool IsKeyHeld(const eInputKey key) const noexcept
        {
            return mKeyState[key] && mPreviousKeyState[key];
        }

        [[nodiscard]] constexpr bool IsKeyDown(const eInputKey key) const noexcept
        {
            return mKeyState[key];
        }

        [[nodiscard]] constexpr bool IsKeyReleased(const eInputKey key) const noexcept
        {
            return !mKeyState[key] && mPreviousKeyState[key];
        }

    private:


        //std::bitset<InputKeyCount> mLiveKeyState = {};
        InputKeyState mKeyState = {};
        InputKeyState mPreviousKeyState = {};
    };

    enum class eGamepadButton : unsigned char
    {
        DPadUp,
        DPadDown,
        DPadLeft,
        DPadRight,
        Start,
        Back,
        LeftThumb,
        RightThumb,
        LeftShoulder,
        RightShoulder,
        A,
        B,
        X,
        Y
    };

    struct InputGamepadState final
    {
        static constexpr std::size_t ButtonCount = 16;

        [[nodiscard]] constexpr decltype(auto) operator[](const eGamepadButton button) noexcept
        {
            return mButtonState[static_cast<std::size_t>(button)];
        }
        [[nodiscard]] constexpr decltype(auto) operator[](const eGamepadButton button) const noexcept
        {
            return mButtonState[static_cast<std::size_t>(button)];
        }

        std::bitset<ButtonCount> mButtonState = {};
        Vector2<int16_t> mLeftThumb{};
        Vector2<int16_t> mRightThumb{};
        uint8_t mLeftTriggerValue{};
        uint8_t mRightTriggerValue{};
    };

    struct InputGamepadFrameState final
    {
        static constexpr std::size_t ButtonCount = 16;


        void Update(const InputGamepadState& newState)
        {
            mPreviousState = mState;
            mState = newState;
        }

        [[nodiscard]] constexpr decltype(auto) operator[](const eGamepadButton button) noexcept
        {
            return mState[button];
        }
        [[nodiscard]] constexpr decltype(auto) operator[](const eGamepadButton button) const noexcept
        {
            return mState[button];
        }

        [[nodiscard]] constexpr bool IsKeyHeld(const eGamepadButton button) const noexcept
        {
            return mState[button] && mPreviousState[button];
        }

        [[nodiscard]] constexpr bool IsPressed(const eGamepadButton button) const noexcept
        {
            return mState[button] && !mPreviousState[button];
        }

        [[nodiscard]] constexpr bool IsReleased(const eGamepadButton button) const noexcept
        {
            return !mState[button] && mPreviousState[button];
        }

        [[nodiscard]] constexpr bool IsKeyDown(const eGamepadButton button) const noexcept
        {
            return mState[button];
        }

        [[nodiscard]] constexpr Vector2<int16_t> GetLeftThumbDirection() const
        {
            return mState.mLeftThumb;
        }

        [[nodiscard]] constexpr Vector2<int16_t> GetRightThumbDirection() const
        {
            return mState.mLeftThumb;
        }

        [[nodiscard]] constexpr uint8_t GetLeftTriggerValue() const
        {
            return mState.mLeftTriggerValue;
        }

        [[nodiscard]] constexpr uint8_t GetRightTriggerValue() const
        {
            return mState.mRightTriggerValue;
        }

        InputGamepadState mState = {};
        InputGamepadState mPreviousState = {};
    };

    using GamepadID = uint32_t;

    class InputState final
    {
    public:

        void Update(Point2i mousePos, Vector2i mousePosDelta, int mouseWheelDelta, const InputKeyState& newInputKeyState, const std::array<InputGamepadState, 4>& newGamepadStates);
        void Reset();

        [[nodiscard]] constexpr bool IsAnyPressed() const;

        [[nodiscard]] constexpr bool IsKeyPressed(const eInputKey key) const noexcept;
        [[nodiscard]] constexpr bool IsKeyHeld(const eInputKey key) const noexcept;
        [[nodiscard]] constexpr bool IsKeyDown(const eInputKey key) const noexcept;
        [[nodiscard]] constexpr bool IsKeyReleased(const eInputKey key) const noexcept;

        // Window coordinates
        //[[nodiscard]] constexpr Point2i GetMousePosition() const noexcept;
        [[nodiscard]] constexpr Vector2i GetMousePositionDelta() const noexcept;
        [[nodiscard]] constexpr int GetMouseWheelDelta() const noexcept;

        [[nodiscard]] const InputGamepadFrameState& GetGamepadState(GamepadID id) const
        {
            return mGamepadStates.at(id);
        }

    private:

        InputKeyFrameState mKeyState;
        InputMouseState mMouseState;
        std::array<InputGamepadFrameState, 4> mGamepadStates;
    };

    inline void InputState::Update(Point2i mousePos, Vector2i mousePosDelta, int mouseWheelDelta, const InputKeyState& newInputKeyState, const std::array<InputGamepadState, 4>& newGamepadStates)
    {
        mKeyState.Update(newInputKeyState);
        mMouseState.Update(mousePos, mousePosDelta, mouseWheelDelta);
        for (std::size_t i = 0; i < mGamepadStates.size(); ++i)
        {
            mGamepadStates[i].Update(newGamepadStates[i]);
        }
    }

    inline void InputState::Reset()
    {
        mKeyState.Reset();
    }

    constexpr bool InputState::IsAnyPressed() const
    {
        return mKeyState.IsAnyPressed();
    }

    constexpr bool InputState::IsKeyPressed(const eInputKey key) const noexcept
    {
        return mKeyState.IsKeyPressed(key);
    }

    constexpr bool InputState::IsKeyHeld(const eInputKey key) const noexcept
    {
        return mKeyState.IsKeyHeld(key);
    }

    constexpr bool InputState::IsKeyDown(const eInputKey key) const noexcept
    {
        return mKeyState.IsKeyDown(key);
    }

    constexpr bool InputState::IsKeyReleased(const eInputKey key) const noexcept
    {
        return mKeyState.IsKeyReleased(key);
    }

    /*constexpr Point2i InputState::GetMousePosition() const noexcept
    {
        return mMouseState.GetPosition();
    }*/

    constexpr Vector2i InputState::GetMousePositionDelta() const noexcept
    {
        return mMouseState.GetPositionDelta();
    }

    constexpr int InputState::GetMouseWheelDelta() const noexcept
    {
        return mMouseState.GetWheelDelta();
    }

}