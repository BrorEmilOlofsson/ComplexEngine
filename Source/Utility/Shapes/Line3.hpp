#pragma once
#include "Utility/Math/Point3.hpp"
#include "Utility/Math/UnitVector3.hpp"

namespace Simple
{

	template<typename T>
	class Line3 final
	{
	public:

		constexpr Line3() = default;

		[[nodiscard]] static constexpr Line3 FromPoints(const Point3<T>& point1, const Point3<T>& point2);
		[[nodiscard]] static constexpr Line3 FromPointAndDirection(const Point3<T>& point, const UnitVector3<T>& direction);
        [[nodiscard]] static constexpr Line3 FromPointAndDirection(const Point3<T>& point, const Vector3<T>& direction);

		constexpr void SetPoint(const Point3<T>& point);
		constexpr void SetDirection(const UnitVector3<T>& direction);

		[[nodiscard]] constexpr const Point3<T>& GetPoint() const noexcept;
		[[nodiscard]] constexpr const UnitVector3<T>& GetDirection() const noexcept;

	private:

		constexpr Line3(const Point3<T>& point, const UnitVector3<T>& direction) noexcept;

	private:

		Point3<T> mPoint;
		UnitVector3<T> mDirection = UnitVector3<T>::Forward();
	};

	using Line3f = Line3<float>;
	using Line3d = Line3<double>;

	template<typename T>
	constexpr Line3<T>::Line3(const Point3<T>& point, const UnitVector3<T>& direction) noexcept
		: mPoint(point)
		, mDirection(direction)
	{
	}

	template<typename T>
	constexpr Line3<T> Line3<T>::FromPoints(const Point3<T>& point1, const Point3<T>& point2)
	{
        return Line3<T>(point1, UnitVector3<T>(point2 - point1));
	}

	template<typename T>
	constexpr Line3<T> Line3<T>::FromPointAndDirection(const Point3<T>& point, const UnitVector3<T>& direction)
	{
		return Line3<T>(point, direction);
	}

	template<typename T>
	constexpr Line3<T> Line3<T>::FromPointAndDirection(const Point3<T>& point, const Vector3<T>& direction)
	{
        return Line3<T>(point, UnitVector3<T>(direction));
	}

	template<typename T>
	constexpr void Line3<T>::SetPoint(const Point3<T>& point)
	{
		mPoint = point;
	}

	template<typename T>
	constexpr void Line3<T>::SetDirection(const UnitVector3<T>& direction)
	{
		mDirection = direction;
	}

	template<typename T>
	constexpr const Point3<T>& Line3<T>::GetPoint() const noexcept
	{
		return mPoint;
	}

	template<typename T>
	constexpr const UnitVector3<T>& Line3<T>::GetDirection() const noexcept
	{
		return mDirection;
	}
}