#pragma once
#include "Utility/Shapes/Plane.hpp"

namespace Simple
{
	enum class eFrustrumSide : std::uint8_t
	{
		Left,
		Right,
		Bottom,
		Top,
		Near,
		Far
	};

	template<typename T>
	class Frustrum final
	{
	public:

		constexpr Frustrum() = default;


		[[nodiscard]] constexpr Plane<T>& operator[](const eFrustrumSide side) noexcept;
		[[nodiscard]] constexpr const Plane<T>& operator[](const eFrustrumSide side) const noexcept;

	private:

		Plane<T> mLeftPlane;
		Plane<T> mRightPlane;
		Plane<T> mBottomPlane;
		Plane<T> mTopPlane;
		Plane<T> mNearPlane;
		Plane<T> mFarPlane;
	};

	using Frustrumf = Frustrum<float>;
	using Frustrumd = Frustrum<double>;

	template<typename T>
	constexpr Plane<T>& Frustrum<T>::operator[](const eFrustrumSide side) noexcept
	{
		switch (side)
		{
		case eFrustrumSide::Left:
			return mLeftPlane;
		case eFrustrumSide::Right:
			return mRightPlane;
		case eFrustrumSide::Bottom:
			return mBottomPlane;
		case eFrustrumSide::Top:
			return mTopPlane;
		case eFrustrumSide::Near:
			return mNearPlane;
		case eFrustrumSide::Far:
			return mFarPlane;
		default:
			return mLeftPlane;
		}
	}
	template<typename T>
	constexpr const Plane<T>& Frustrum<T>::operator[](const eFrustrumSide side) const noexcept
	{
		switch (side)
		{
		case eFrustrumSide::Left:
			return mLeftPlane;
		case eFrustrumSide::Right:
			return mRightPlane;
		case eFrustrumSide::Bottom:
			return mBottomPlane;
		case eFrustrumSide::Top:
			return mTopPlane;
		case eFrustrumSide::Near:
			return mNearPlane;
		case eFrustrumSide::Far:
			return mFarPlane;
		default:
			return mLeftPlane;
		}
	}
}