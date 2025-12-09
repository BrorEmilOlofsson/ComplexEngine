#pragma once
#include "Utility/Math/Point2.hpp"
#include "Utility/Math/UnitVector2.hpp"

namespace Simple
{
	template<typename T>
	class Line2 final
	{
	public:

		constexpr Line2() = default;

		[[nodiscard]] static constexpr Line2 FromPoints(const Point2<T>& point1, const Point2<T>& point2);
        [[nodiscard]] static constexpr Line2 FromPointAndDirection(const Point2<T>& point, const UnitVector2<T>& direction);
        [[nodiscard]] static constexpr Line2 FromPointAndDirection(const Point2<T>& point, const Vector2<T>& direction);

		constexpr void SetPoint(const Point2<T>& point);
		constexpr void SetDirection(const UnitVector2<T>& direction);

		[[nodiscard]] constexpr const Point2<T>& GetPoint() const;
		[[nodiscard]] constexpr const UnitVector2<T>& GetDirection() const;

	private:

		constexpr Line2(const Point2<T>& point, const UnitVector2<T>& direction);

	private:

		Point2<T> mPoint;
		UnitVector2<T> mDirection = UnitVector2<T>::Right();
	};

	using Line2f = Line2<float>;
	using Line2d = Line2<double>;

	template<typename T>
	constexpr Line2<T>::Line2(const Point2<T>& point, const UnitVector2<T>& direction)
		: mPoint(point)
		, mDirection(direction)
	{
	}

	template<typename T>
	constexpr Line2<T> Line2<T>::FromPoints(const Point2<T>& point1, const Point2<T>& point2)
	{
        return Line2<T>(point1, UnitVector2<T>(point2 - point1));
	}

	template<typename T>
	constexpr Line2<T> Line2<T>::FromPointAndDirection(const Point2<T>& point, const UnitVector2<T>& direction)
	{
		return Line2<T>(point, direction);
	}

	template<typename T>
	constexpr Line2<T> Line2<T>::FromPointAndDirection(const Point2<T>& point, const Vector2<T>& direction)
	{
        return Line2<T>(point, UnitVector2<T>(direction));
	}

	template<typename T>
	constexpr void Line2<T>::SetPoint(const Point2<T>& point)
	{
		mPoint = point;
	}

	template<typename T>
	constexpr void Line2<T>::SetDirection(const UnitVector2<T>& direction)
	{
		mDirection = direction;
	}

	template<typename T>
	constexpr const Point2<T>& Line2<T>::GetPoint() const
	{
		return mPoint;
	}

	template<typename T>
	constexpr const UnitVector2<T>& Line2<T>::GetDirection() const
	{
		return mDirection;
	}
}