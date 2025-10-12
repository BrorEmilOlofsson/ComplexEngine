#pragma once
#include "Utility/Math/Point2.hpp"
#include "Utility/Math/UnitVector2.hpp"

namespace Simple
{

	template<typename T>
	class Ray2 final
	{
	public:

		constexpr Ray2() = default;
		constexpr Ray2(const Point2<T>& origin, const UnitVector2<T>& direction);
		constexpr Ray2(const Point2<T>& origin, const Point2<T>& point);

		constexpr const Point2<T>& GetOrigin() const;
		constexpr const UnitVector2<T>& GetDirection() const;

		constexpr void SetOrigin(const Point2<T>& origin);
		constexpr void SetDirection(const UnitVector2<T>& direction);

		constexpr Point2<T> GetPointAtDistance(const T& distance) const;

	private:

		Point2<T> mOrigin;
		UnitVector2<T> mDirection;
	};

	using Ray2f = Ray2<float>;

	template<typename T>
	constexpr Ray2<T>::Ray2(const Point2<T>& origin, const UnitVector2<T>& direction)
		: mOrigin(origin)
		, mDirection(direction)
	{
	}

	template<typename T>
	constexpr Ray2<T>::Ray2(const Point2<T>& origin, const Point2<T>& point)
		: mOrigin(origin)
		, mDirection(point - origin)
	{
	}

	template<typename T>
	constexpr const Point2<T>& Ray2<T>::GetOrigin() const
	{
		return mOrigin;
	}

	template<typename T>
	constexpr const UnitVector2<T>& Ray2<T>::GetDirection() const
	{
		return mDirection;
	}

	template<typename T>
	constexpr void Ray2<T>::SetOrigin(const Point2<T>& origin)
	{
		mOrigin = origin;
	}

	template<typename T>
	constexpr void Ray2<T>::SetDirection(const UnitVector2<T>& direction)
	{
		mDirection = direction;
	}

	template<typename T>
	constexpr Point2<T> Ray2<T>::GetPointAtDistance(const T& distance) const
	{
		return mOrigin + mDirection * distance;
	}
}