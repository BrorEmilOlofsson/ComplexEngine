#pragma once
#include "Utility/Math/Point3.hpp"

namespace Simple
{

	template<typename T>
	class Sphere final
	{
	public:

		constexpr Sphere() = default;

        [[nodiscard]] static constexpr Sphere<T> FromCenterAndRadius(const Point3<T>& center, const T& radius);
        [[nodiscard]] static constexpr Sphere<T> FromCenter(const Point3<T>& center);
        [[nodiscard]] static constexpr Sphere<T> FromRadius(const T& radius);

		constexpr void SetCenter(const Point3<T>& center);
		constexpr void SetRadius(const T& radius);

		[[nodiscard]] constexpr const Point3<T>& GetCenter() const;
		[[nodiscard]] constexpr T GetRadius() const;
		[[nodiscard]] constexpr T GetDiameter() const;

	private:

		constexpr Sphere(const Point3<T>& center, const T& radius);

	public:

		Point3<T> mCenter = Point3<T>::Zero();
		T mRadius = T{ 0 };
	};

	using Spheref = Sphere<float>;

	template<typename T>
	constexpr Sphere<T>::Sphere(const Point3<T>& center, const T& radius)
		: mCenter(center)
		, mRadius(radius)
	{
	}

	template<typename T>
	constexpr Sphere<T> Sphere<T>::FromCenterAndRadius(const Point3<T>& center, const T& radius)
	{
		return Sphere<T>(center, radius);
	}

	template<typename T>
	constexpr Sphere<T> Sphere<T>::FromCenter(const Point3<T>& center)
	{
		return Sphere<T>(center, T{ 0 });
    }

	template<typename T>
	constexpr Sphere<T> Sphere<T>::FromRadius(const T& radius)
	{
		return Sphere<T>(Point3<T>::Zero(), radius);
	}

	template<typename T>
	constexpr void Sphere<T>::SetCenter(const Point3<T>& center)
	{
		mCenter = center;
	}

	template<typename T>
	constexpr void Sphere<T>::SetRadius(const T& radius)
	{
		mRadius = radius;
	}

	template<typename T>
	constexpr const Point3<T>& Sphere<T>::GetCenter() const
	{
		return mCenter;
	}

	template<typename T>
	constexpr T Sphere<T>::GetRadius() const
	{
		return mRadius;
	}

	template<typename T>
	constexpr T Sphere<T>::GetDiameter() const
	{
		return mRadius * 2;
	}
}