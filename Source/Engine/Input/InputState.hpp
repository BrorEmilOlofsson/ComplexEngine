#pragma once
#include "Engine/Input/InputKey.hpp"
#include "Engine/Input/InputMouseState.hpp"
#include <cstddef>
#include <bitset>

namespace CLX
{

    class InputKeyState final
    {
        static constexpr std::size_t InputKeyCount = 255;
    public:

        InputKeyState() = default;

        void Update()
        {
            mPreviousKeyState = mKeyState;
            mKeyState = mLiveKeyState;
        }

        void Reset()
        {
            mPreviousKeyState.reset();
            mKeyState.reset();
            mLiveKeyState.reset();
        }

        [[nodiscard]] constexpr bool IsAnyPressed() const
        {
            return mKeyState.any();
        }

        [[nodiscard]] constexpr decltype(auto) operator[](const eInputKey key) noexcept
        {
            return mLiveKeyState[static_cast<std::size_t>(key)];
        }

        [[nodiscard]] constexpr bool IsKeyPressed(const eInputKey key) const noexcept
        {
            return mKeyState[static_cast<std::size_t>(key)] && !mPreviousKeyState[static_cast<std::size_t>(key)];
        }

        [[nodiscard]] constexpr bool IsKeyHeld(const eInputKey key) const noexcept
        {
            return mKeyState[static_cast<std::size_t>(key)] && mPreviousKeyState[static_cast<std::size_t>(key)];
        }

        [[nodiscard]] constexpr bool IsKeyDown(const eInputKey key) const noexcept
        {
            return mKeyState[static_cast<std::size_t>(key)];
        }

        [[nodiscard]] constexpr bool IsKeyReleased(const eInputKey key) const noexcept
        {
            return !mKeyState[static_cast<std::size_t>(key)] && mPreviousKeyState[static_cast<std::size_t>(key)];
        }

    private:


        std::bitset<InputKeyCount> mLiveKeyState = {};
        std::bitset<InputKeyCount> mKeyState = {};
        std::bitset<InputKeyCount> mPreviousKeyState = {};
    };

    struct GamepadInputState
    {
        static constexpr std::size_t ButtonCount = 16;

    };

    using GamepadID = std::size_t;

    class InputState final
    {
    public:

        void Update(Point2i mousePos, Vector2i mousePosDelta, int mouseWheelDelta);
        void Reset();

        [[nodiscard]] constexpr bool IsAnyPressed() const;

        [[nodiscard]] constexpr decltype(auto) operator[](const eInputKey key) noexcept;
        [[nodiscard]] constexpr bool IsKeyPressed(const eInputKey key) const noexcept;
        [[nodiscard]] constexpr bool IsKeyHeld(const eInputKey key) const noexcept;
        [[nodiscard]] constexpr bool IsKeyDown(const eInputKey key) const noexcept;
        [[nodiscard]] constexpr bool IsKeyReleased(const eInputKey key) const noexcept;

        // Window coordinates
        //[[nodiscard]] constexpr Point2i GetMousePosition() const noexcept;
        [[nodiscard]] constexpr Vector2i GetMousePositionDelta() const noexcept;
        [[nodiscard]] constexpr int GetMouseWheelDelta() const noexcept;

        [[nodiscard]] const GamepadInputState& GetGamepadState(GamepadID id) const
        {
            return mGamepadStates[id];
        }

    private:

        InputKeyState mKeyState;
        InputMouseState mMouseState;
        std::array<GamepadInputState, 4> mGamepadStates;
    };

    inline void InputState::Update(Point2i mousePos, Vector2i mousePosDelta, int mouseWheelDelta)
    {
        mKeyState.Update();
        mMouseState.Update(mousePos, mousePosDelta, mouseWheelDelta);
    }

    inline void InputState::Reset()
    {
        mKeyState.Reset();
    }

    constexpr bool InputState::IsAnyPressed() const
    {
        return mKeyState.IsAnyPressed();
    }

    constexpr decltype(auto) InputState::operator[](const eInputKey key) noexcept
    {
        return mKeyState[key];
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