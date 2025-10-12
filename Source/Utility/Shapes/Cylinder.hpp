#pragma once
#include "Utility/Math/Point3.hpp"
#include "Utility/Math/UnitVector3.hpp"

namespace Simple
{
	
	template<typename T>
	class Cylinder final
	{
	public:

		constexpr Cylinder() = default;
		constexpr Cylinder(const Point3<T>& center, const T radius, const UnitVector3<T>& axis, const T height);

		constexpr void SetCenter(const Point3<T>& center);
		constexpr void SetRadius(const T radius);
		constexpr void SetAxis(const UnitVector3<T>& axis);
		constexpr void SetHeight(const T height);

		[[nodiscard]] constexpr const Point3<T>& GetCenter() const;
		[[nodiscard]] constexpr T GetRadius() const;
		[[nodiscard]] constexpr const UnitVector3<T>& GetAxis() const;
		[[nodiscard]] constexpr T GetHeight() const;

		[[nodiscard]] constexpr Point3<T> GetLowerPoint() const;
		[[nodiscard]] constexpr Point3<T> GetUpperPoint() const;

	private:

		Point3<T> mCenter;
		T mRadius = T{ 0.5 };
		UnitVector3<T> mAxis = UnitVector3f::Up();
		T mHeight = T{ 1 };
	};

	using Cylinderf = Cylinder<float>;

	template<typename T>
	constexpr Cylinder<T>::Cylinder(const Point3<T>& center, const T radius, const UnitVector3<T>& axis, const T height)
		: mCenter(center)
		, mRadius(radius)
		, mAxis(axis)
		, mHeight(height)
	{
	}

	template<typename T>
	constexpr void Cylinder<T>::SetCenter(const Point3<T>& center)
	{
		mCenter = center;
	}

	template<typename T>
	constexpr void Cylinder<T>::SetRadius(const T radius)
	{
		mRadius = radius;
	}

	template<typename T>
	constexpr void Cylinder<T>::SetAxis(const UnitVector3<T>& axis)
	{
		mAxis = axis;
	}

	template<typename T>
	constexpr void Cylinder<T>::SetHeight(const T height)
	{
		mHeight = height;
	}

	template<typename T>
	constexpr const Point3<T>& Cylinder<T>::GetCenter() const
	{
		return mCenter;
	}

	template<typename T>
	constexpr T Cylinder<T>::GetRadius() const
	{
		return mRadius;
	}

	template<typename T>
	constexpr const UnitVector3<T>& Cylinder<T>::GetAxis() const
	{
		return mAxis;
	}

	template<typename T>
	constexpr T Cylinder<T>::GetHeight() const
	{
		return mHeight;
	}

	template<typename T>
	constexpr Point3<T> Cylinder<T>::GetLowerPoint() const
	{
		return mCenter - mAxis * (mHeight / T{ 2 });
	}

	template<typename T>
	constexpr Point3<T> Cylinder<T>::GetUpperPoint() const
	{
		return mCenter + mAxis * (mHeight / T{ 2 });
	}
}