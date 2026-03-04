#pragma once
#include "Engine/Math/Point2.hpp"
#include "Engine/Math/UnitVector2.hpp"

namespace Simple
{

	template<typename T>
	class Ray2 final
	{
	public:

		constexpr Ray2() = default;

		[[nodiscard]] static constexpr Ray2<T> FromOriginAndDirection(const Point2<T>& origin, const UnitVector2<T>& direction);
        [[nodiscard]] static constexpr Ray2<T> FromOriginAndPoint(const Point2<T>& origin, const Point2<T>& point);

		[[nodiscard]] constexpr const Point2<T>& GetOrigin() const;
		[[nodiscard]] constexpr const UnitVector2<T>& GetDirection() const;

		constexpr void SetOrigin(const Point2<T>& origin);
		constexpr void SetDirection(const UnitVector2<T>& direction);

		[[nodiscard]] constexpr Point2<T> GetPointAtDistance(const T& distance) const;

	private:

		constexpr Ray2(const Point2<T>& origin, const UnitVector2<T>& direction);
		
	private:

		Point2<T> mOrigin;
		UnitVector2<T> mDirection = UnitVector2d::Right();
	};

	using Ray2f = Ray2<float>;
	using Ray2d = Ray2<double>;

	template<typename T>
	constexpr Ray2<T>::Ray2(const Point2<T>& origin, const UnitVector2<T>& direction)
		: mOrigin(origin)
		, mDirection(direction)
	{
	}

	template<typename T>
	constexpr Ray2<T> Ray2<T>::FromOriginAndDirection(const Point2<T>& origin, const UnitVector2<T>& direction)
	{
		return Ray2<T>(origin, direction);
    }

	template<typename T>
	constexpr Ray2<T> Ray2<T>::FromOriginAndPoint(const Point2<T>& origin, const Point2<T>& point)
	{
		return Ray2<T>(origin, UnitVector2<T>(point - origin));
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

    template<typename T>
	[[nodiscard]] constexpr bool operator==(const Ray2<T>& a, const Ray2<T>& b) noexcept
	{
		return a.GetOrigin() == b.GetOrigin() && a.GetDirection() == b.GetDirection();
    }
}