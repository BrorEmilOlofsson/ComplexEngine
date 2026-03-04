#pragma once
#include "Engine/Math/Point2.hpp"
#include "Engine/Math/Math.hpp"

namespace Simple
{

	class InputMouseState final
	{
	public:

		InputMouseState() = default;

		constexpr void Update(Point2i position, Vector2i deltaPos, int wheelDelta);

		[[nodiscard]] constexpr int GetWheelDelta() const;
		[[nodiscard]] constexpr Vector2i GetPositionDelta() const;
		[[nodiscard]] constexpr Point2i GetPosition() const;

	private:

		Point2i mPosition; // Relative to Client/HWND Window
		Point2i mPreviousPositon;
		Vector2i mPositionDelta;
		int mWheelDelta = 0;
	};

	constexpr void InputMouseState::Update(Point2i position, Vector2i deltaPos, int wheelDelta)
	{
		mPreviousPositon = mPosition;
		mPosition = position;
		mPositionDelta = deltaPos;
		mWheelDelta = Sign(wheelDelta);
	}

	constexpr int InputMouseState::GetWheelDelta() const
	{
		return mWheelDelta;
	}

	constexpr Point2i InputMouseState::GetPosition() const
	{
		return mPosition;
	}

	constexpr Vector2i InputMouseState::GetPositionDelta() const
	{
		return mPositionDelta;
	}
}