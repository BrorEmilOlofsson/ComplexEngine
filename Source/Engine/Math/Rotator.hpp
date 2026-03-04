#pragma once
#include "Engine/Math/Angle.hpp"
#include "Engine/Math/Math.hpp"

namespace CLX
{
	template<typename T>
	class Rotator final
	{
	public:

		constexpr Rotator() = default;
		constexpr Rotator(const Degrees<T>& pitch, const Degrees<T>& yaw, const Degrees<T>& roll);
		constexpr Rotator(const Radians<T>& pitch, const Radians<T>& yaw, const Radians<T>& roll);

		[[nodiscard]] constexpr Degrees<T>& Pitch();
		[[nodiscard]] constexpr Degrees<T>& Yaw();
		[[nodiscard]] constexpr Degrees<T>& Roll();

		[[nodiscard]] constexpr const Degrees<T>& Pitch() const;
		[[nodiscard]] constexpr const Degrees<T>& Yaw() const;
		[[nodiscard]] constexpr const Degrees<T>& Roll() const;

		// Normalize the angles to be between -180 and 180 degrees
		constexpr void Normalize180();

		// Normalize the angles to be between 0 and 360 degrees
		constexpr void Normalize360();

	private:

		Degrees<T> mPitch; // x
		Degrees<T> mYaw;   // y
		Degrees<T> mRoll;  // z
	};

	using Rotatorf = Rotator<float>;
	using Rotatord = Rotator<double>;

	template<typename T>
	constexpr Rotator<T>::Rotator(const Degrees<T>& pitch, const Degrees<T>& yaw, const Degrees<T>& roll)
		: mPitch(pitch)
		, mYaw(yaw)
		, mRoll(roll)
	{
	}

	template<typename T>
	constexpr Rotator<T>::Rotator(const Radians<T>& pitch, const Radians<T>& yaw, const Radians<T>& roll)
		: mPitch(ToDegrees(pitch))
		, mYaw(ToDegrees(yaw))
		, mRoll(ToDegrees(roll))
	{
	}

	template<typename T>
	constexpr Degrees<T>& Rotator<T>::Pitch()
	{
		return mPitch;
	}

	template<typename T>
	constexpr Degrees<T>& Rotator<T>::Yaw()
	{
		return mYaw;
	}

	template<typename T>
	constexpr Degrees<T>& Rotator<T>::Roll()
	{
		return mRoll;
	}

	template<typename T>
	constexpr const Degrees<T>& Rotator<T>::Pitch() const
	{
		return mPitch;
	}

	template<typename T>
	constexpr const Degrees<T>& Rotator<T>::Yaw() const
	{
		return mYaw;
	}

	template<typename T>
	constexpr const Degrees<T>& Rotator<T>::Roll() const
	{
		return mRoll;
	}

	template<typename T>
	constexpr Degrees<T> NormalizeAngle(Degrees<T> angle)
	{
		while (angle > Degrees<T>(180))
		{
			angle -= Degrees<T>(360);
		}
		while (angle < Degrees<T>(-180))
		{
			angle += Degrees<T>(360);
		}
		return angle;
	}

	template<typename T>
	constexpr void Rotator<T>::Normalize180()
	{
		mPitch = NormalizeAngle(mPitch);
		mYaw = NormalizeAngle(mYaw);
		mRoll = NormalizeAngle(mRoll);
	}

	template<typename T>
	constexpr void Rotator<T>::Normalize360()
	{
		Normalize180();
		if (mPitch.Value() < 0)
			mPitch.Value() += 360;
		if (mYaw.Value() < 0)
			mYaw.Value() += 360;
		if (mRoll.Value() < 0)
			mRoll.Value() += 360;
	}

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const Rotator<T>& a, const Rotator<T>& b) noexcept
	{
		return a.Pitch() == b.Pitch()
			&& a.Yaw() == b.Yaw()
			&& a.Roll() == b.Roll();
	}
}