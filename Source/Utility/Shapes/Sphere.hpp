#pragma once
#include "Utility/Math/Point3.hpp"
#include "Utility/Math/GeometryMath.hpp"

namespace Simple
{

	template<typename T>
	class Sphere final
	{
	public:

		constexpr Sphere() = default;

        [[nodiscard]] static constexpr Sphere<T> FromCenterAndRadius(const Point3<T>& center, const Radius<T>& radius);
        [[nodiscard]] static constexpr Sphere<T> FromCenter(const Point3<T>& center);
        [[nodiscard]] static constexpr Sphere<T> FromRadius(const Radius<T>& radius);

		constexpr void SetCenter(const Point3<T>& center);
		constexpr void SetRadius(const Radius<T>& radius);

		[[nodiscard]] constexpr const Point3<T>& GetCenter() const;
		[[nodiscard]] constexpr Radius<T> GetRadius() const;
		[[nodiscard]] constexpr Diameter<T> GetDiameter() const;

	private:

		constexpr Sphere(const Point3<T>& center, const Radius<T>& radius);

	public:

		Point3<T> mCenter = Point3<T>::Zero();
		Radius<T> mRadius = Radius<T>{ 0 };
	};

	using Spheref = Sphere<float>;
	using Sphered = Sphere<double>;

	template<typename T>
	constexpr Sphere<T>::Sphere(const Point3<T>& center, const Radius<T>& radius)
		: mCenter(center)
		, mRadius(radius)
	{
	}

	template<typename T>
	constexpr Sphere<T> Sphere<T>::FromCenterAndRadius(const Point3<T>& center, const Radius<T>& radius)
	{
		return Sphere<T>(center, radius);
	}

	template<typename T>
	constexpr Sphere<T> Sphere<T>::FromCenter(const Point3<T>& center)
	{
		return Sphere<T>(center, Radius<T>{ 0 });
    }

	template<typename T>
	constexpr Sphere<T> Sphere<T>::FromRadius(const Radius<T>& radius)
	{
		return Sphere<T>(Point3<T>::Zero(), radius);
	}

	template<typename T>
	constexpr void Sphere<T>::SetCenter(const Point3<T>& center)
	{
		mCenter = center;
	}

	template<typename T>
	constexpr void Sphere<T>::SetRadius(const Radius<T>& radius)
	{
		mRadius = radius;
	}

	template<typename T>
	constexpr const Point3<T>& Sphere<T>::GetCenter() const
	{
		return mCenter;
	}

	template<typename T>
	constexpr Radius<T> Sphere<T>::GetRadius() const
	{
		return mRadius;
	}

	template<typename T>
	constexpr Diameter<T> Sphere<T>::GetDiameter() const
	{
		return ToDiameter(mRadius);
	}
}