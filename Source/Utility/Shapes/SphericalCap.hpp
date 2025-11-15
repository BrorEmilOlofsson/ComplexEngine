#pragma once
#include "Utility/Math/UnitVector3.hpp"
#include "Utility/Shapes/Sphere.hpp"

namespace Simple
{

	template<typename T>
	class SphericalCap final
	{
	public:

		constexpr SphericalCap() = default;
		constexpr SphericalCap(const Sphere<T>& sphere, const UnitVector3<T>& normal, const T height)
			: mSphere(sphere)
			, mNormal(normal)
			, mHeight(height)
		{
			if (height > mSphere.GetDiameter())
			{
				throw std::invalid_argument("SphericalCap height cannot be greater than the diameter of the sphere.");
			}
		}
		
		[[nodiscard]] constexpr const Point3<T>& GetSphereCenter() const noexcept;
		[[nodiscard]] constexpr T GetSphereRadius() const noexcept;
		[[nodiscard]] constexpr const UnitVector3<T>& GetNormal() const noexcept;
		[[nodiscard]] constexpr T GetHeight() const noexcept;

	private:

		Sphere<T> mSphere;
		UnitVector3<T> mNormal = UnitVector3<T>::Up();
		T mHeight = T{};
	};

	using SphericalCapf = SphericalCap<float>;

	template<typename T>
	constexpr const Point3<T>& SphericalCap<T>::GetSphereCenter() const noexcept
	{
		return mSphere.GetCenter();
	}

	template<typename T>
	constexpr T SphericalCap<T>::GetSphereRadius() const noexcept
	{
		return mSphere.GetRadius();
	}

	template<typename T>
	constexpr const UnitVector3<T>& SphericalCap<T>::GetNormal() const noexcept
	{
		return mNormal;
	}

	template<typename T>
	constexpr T SphericalCap<T>::GetHeight() const noexcept
	{
		return mHeight;
	}
}