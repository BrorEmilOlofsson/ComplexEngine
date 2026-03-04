#pragma once
#include "Engine/Math/Point3.hpp"

namespace Simple
{

	template<typename T>
	class LineSegment3 final
	{
	public:

		constexpr LineSegment3() = default;

		[[nodiscard]] static constexpr LineSegment3<T> FromPoints(const Point3<T>& startPoint, const Point3<T>& endPoint);

		[[nodiscard]] constexpr Point3<T>& StartPoint();
		[[nodiscard]] constexpr Point3<T>& EndPoint();

		[[nodiscard]] constexpr const Point3<T>& StartPoint() const;
		[[nodiscard]] constexpr const Point3<T>& EndPoint() const;

	private:

		constexpr LineSegment3(const Point3<T>& startPoint, const Point3<T>& endPoint);

	private:

		Point3<T> mStartPoint;
		Point3<T> mEndPoint;
	};

	using LineSegment3f = LineSegment3<float>;
	using LineSegment3d = LineSegment3<double>;

	template<typename T>
	constexpr LineSegment3<T>::LineSegment3(const Point3<T>& startPoint, const Point3<T>& endPoint)
		: mStartPoint(startPoint)
		, mEndPoint(endPoint)
	{
	}

	template<typename T>
	constexpr LineSegment3<T> LineSegment3<T>::FromPoints(const Point3<T>& startPoint, const Point3<T>& endPoint)
	{
		return LineSegment3<T>(startPoint, endPoint);
    }

	template<typename T>
	constexpr Point3<T>& LineSegment3<T>::StartPoint()
	{
		return mStartPoint;
	}

	template<typename T>
	constexpr Point3<T>& LineSegment3<T>::EndPoint()
	{
		return mEndPoint;
	}

	template<typename T>
	constexpr const Point3<T>& LineSegment3<T>::StartPoint() const
	{
		return mStartPoint;
	}

	template<typename T>
	constexpr const Point3<T>& LineSegment3<T>::EndPoint() const
	{
		return mEndPoint;
	}

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const LineSegment3<T>& a, const LineSegment3<T>& b) noexcept
	{
		return a.StartPoint() == b.StartPoint() && a.EndPoint() == b.EndPoint();
    }
}