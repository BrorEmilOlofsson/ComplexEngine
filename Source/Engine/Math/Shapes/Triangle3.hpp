#pragma once
#include "Engine/Math/Point3.hpp"

namespace CLX
{
	
	template<typename T>
	class Triangle3 final
	{
	public:
		
		constexpr Triangle3() = default;
		constexpr Triangle3(const Point3<T>& point0, const Point3<T>& point1, const Point3<T>& point2);

		[[nodiscard]] constexpr Point3<T>& GetPoint0() noexcept;
		[[nodiscard]] constexpr Point3<T>& GetPoint1() noexcept;
		[[nodiscard]] constexpr Point3<T>& GetPoint2() noexcept;

		[[nodiscard]] constexpr const Point3<T>& GetPoint0() const noexcept;
		[[nodiscard]] constexpr const Point3<T>& GetPoint1() const noexcept;
		[[nodiscard]] constexpr const Point3<T>& GetPoint2() const noexcept;

	private:

		Point3<T> mPoint0;
		Point3<T> mPoint1;
		Point3<T> mPoint2;
	};

	using Triangle3f = Triangle3<float>;
	using Triangle3d = Triangle3<double>;

	template<typename T>
	constexpr Triangle3<T>::Triangle3(const Point3<T>& point0, const Point3<T>& point1, const Point3<T>& point2)
		: mPoint0(point0)
		, mPoint1(point1)
		, mPoint2(point2)
	{
	}

	template<typename T>
	constexpr Point3<T>& Triangle3<T>::GetPoint0() noexcept
	{
		return mPoint0;
	}

	template<typename T>
	constexpr Point3<T>& Triangle3<T>::GetPoint1() noexcept
	{
		return mPoint1;
	}

	template<typename T>
	constexpr Point3<T>& Triangle3<T>::GetPoint2() noexcept
	{
		return mPoint2;
	}

	template<typename T>
	constexpr const Point3<T>& Triangle3<T>::GetPoint0() const noexcept
	{
		return mPoint0;
	}

	template<typename T>
	constexpr const Point3<T>& Triangle3<T>::GetPoint1() const noexcept
	{
		return mPoint1;
	}

	template<typename T>
	constexpr const Point3<T>& Triangle3<T>::GetPoint2() const noexcept
	{
		return mPoint2;
	}
}