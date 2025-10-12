#pragma once
#include "Utility/Math/Point3.hpp"
#include "Utility/Math/UnitVector3.hpp"

namespace Simple
{

	template<typename T>
	class Ray3 final
	{
	public:

		constexpr Ray3() = default;
		constexpr Ray3(const Point3<T>& origin, const UnitVector3<T>& direction);
		constexpr Ray3(const Point3<T>& origin, const Point3<T>& point);

		constexpr const Point3<T>& GetOrigin() const;
		constexpr const UnitVector3<T>& GetDirection() const;

		constexpr void SetOrigin(const Point3<T>& origin);
		constexpr void SetDirection(const UnitVector3<T>& direction);

		constexpr Point3<T> GetPointAtDistance(const T& distance) const;

	private:

		Point3<T> mOrigin;
		UnitVector3<T> mDirection;
	};

	using Ray3f = Ray3<float>;

	template<typename T>
	constexpr Ray3<T>::Ray3(const Point3<T>& origin, const UnitVector3<T>& direction)
		: mOrigin(origin)
		, mDirection(direction)
	{
	}

	template<typename T>
	constexpr Ray3<T>::Ray3(const Point3<T>& origin, const Point3<T>& point)
		: mOrigin(origin)
		, mDirection(point - origin)
	{
	}

	template<typename T>
	constexpr const Point3<T>& Ray3<T>::GetOrigin() const
	{
		return mOrigin;
	}

	template<typename T>
	constexpr const UnitVector3<T>& Ray3<T>::GetDirection() const
	{
		return mDirection;
	}

	template<typename T>
	constexpr void Ray3<T>::SetOrigin(const Point3<T>& origin)
	{
		mOrigin = origin;
	}

	template<typename T>
	constexpr void Ray3<T>::SetDirection(const UnitVector3<T>& direction)
	{
		mDirection = direction;
	}

	template<typename T>
	constexpr Point3<T> Ray3<T>::GetPointAtDistance(const T& distance) const
	{
		return mOrigin + mDirection * distance;
	}
}