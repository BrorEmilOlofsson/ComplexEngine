#pragma once
#include "Utility/Math/Point3.hpp"
#include "Utility/Math/UnitVector3.hpp"

namespace Simple
{
	
	template<typename T>
	class Disk final
	{
	public:

		constexpr Disk() = default;
		constexpr Disk(const Point3<T>& center, const UnitVector3<T>& normal, const T radius);

		constexpr void SetCenter(const Point3<T>& center);
		constexpr void SetNormal(const UnitVector3<T>& normal);
		constexpr void SetRadius(const T radius);

		[[nodiscard]] constexpr const Point3<T>& GetCenter() const;
		[[nodiscard]] constexpr const UnitVector3<T>& GetNormal() const;
		[[nodiscard]] constexpr T GetRadius() const;

	private:

		Point3<T> mCenter;
		UnitVector3<T> mNormal;
		T mRadius = T{};
	};

	using Diskf = Disk<float>;

	template<typename T>
	constexpr Disk<T>::Disk(const Point3<T>& center, const UnitVector3<T>& normal, const T radius)
		: mCenter(center)
		, mNormal(normal)
		, mRadius(radius)
	{
	}

	template<typename T>
	constexpr void Disk<T>::SetCenter(const Point3<T>& center)
	{
		mCenter = center;
	}

	template<typename T>
	constexpr void Disk<T>::SetNormal(const UnitVector3<T>& normal)
	{
		mNormal = normal;
	}

	template<typename T>
	constexpr void Disk<T>::SetRadius(const T radius)
	{
		mRadius = radius;
	}

	template<typename T>
	constexpr const Point3<T>& Disk<T>::GetCenter() const
	{
		return mCenter;
	}

	template<typename T>
	constexpr const UnitVector3<T>& Disk<T>::GetNormal() const
	{
		return mNormal;
	}

	template<typename T>
	constexpr T Disk<T>::GetRadius() const
	{
		return mRadius;
	}
}