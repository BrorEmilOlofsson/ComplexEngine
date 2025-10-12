#pragma once
#include "Utility/Math/Point2.hpp"
#include "Utility/Math/UnitVector2.hpp"

namespace Simple
{

	template<typename T>
	class LineSegment2 final
	{
	public:

		constexpr LineSegment2() = default;
		constexpr LineSegment2(const Point2<T>& startPoint, const Point2<T>& endPoint);

		[[nodiscard]] constexpr Point2<T>& StartPoint() noexcept;
		[[nodiscard]] constexpr Point2<T>& EndPoint() noexcept;

		[[nodiscard]] constexpr const Point2<T>& StartPoint() const noexcept;
		[[nodiscard]] constexpr const Point2<T>& EndPoint() const noexcept;

	private:

		Point2<T> mStartPoint;
		Point2<T> mEndPoint;
	};

	using LineSegment2f = LineSegment2<float>;

	template<typename T>
	constexpr LineSegment2<T>::LineSegment2(const Point2<T>& startPoint, const Point2<T>& endPoint)
		: mStartPoint(startPoint)
		, mEndPoint(endPoint)
	{
	}

	template<typename T>
	constexpr Point2<T>& LineSegment2<T>::StartPoint() noexcept
	{
		return mStartPoint;
	}

	template<typename T>
	constexpr Point2<T>& LineSegment2<T>::EndPoint() noexcept
	{
		return mEndPoint;
	}

	template<typename T>
	constexpr const Point2<T>& LineSegment2<T>::StartPoint() const noexcept
	{
		return mStartPoint;
	}

	template<typename T>
	constexpr const Point2<T>& LineSegment2<T>::EndPoint() const noexcept
	{
		return mEndPoint;
	}
}