#pragma once
#include "Utility/Input/InputKey.hpp"
#include "Utility/Input/InputMouseState.hpp"
#include <cstddef>
#include <bitset>

namespace Simple
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

		[[nodiscard]] bool IsAnyPressed() const
		{
			return mKeyState.any();
		}

		[[nodiscard]] decltype(auto) operator[](const eInputKey key) noexcept
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

	class InputState final
	{
	public:

		void Update(Point2i mousePos, Vector2i mousePosDelta, int mouseWheelDelta)
		{
			mKeyState.Update();
			mMouseState.Update(mousePos, mousePosDelta, mouseWheelDelta);
		}

		void Reset()
		{
			mKeyState.Reset();
		}

		[[nodiscard]] bool IsAnyPressed() const
		{
			return mKeyState.IsAnyPressed();
		}

		[[nodiscard]] decltype(auto) operator[](const eInputKey key) noexcept
		{
			return mKeyState[key];
		}

		[[nodiscard]] constexpr bool IsKeyPressed(const eInputKey key) const noexcept
		{
			return mKeyState.IsKeyPressed(key);
		}

		[[nodiscard]] constexpr bool IsKeyHeld(const eInputKey key) const noexcept
		{
			return mKeyState.IsKeyHeld(key);
		}

		[[nodiscard]] constexpr bool IsKeyDown(const eInputKey key) const noexcept
		{
			return mKeyState.IsKeyDown(key);
		}

		[[nodiscard]] constexpr bool IsKeyReleased(const eInputKey key) const noexcept
		{
			return mKeyState.IsKeyReleased(key);
		}

		// Window coordinates
		[[nodiscard]] constexpr Point2i GetMousePosition() const noexcept
		{
			return mMouseState.GetPosition();
		}

		[[nodiscard]] constexpr Vector2i GetMousePositionDelta() const noexcept
		{
			return mMouseState.GetPositionDelta();
		}

		[[nodiscard]] constexpr int GetMouseWheelDelta() const noexcept
		{
			return mMouseState.GetWheelDelta();
		}

	private:

		InputKeyState mKeyState;
		InputMouseState mMouseState;
	};


}