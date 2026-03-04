#pragma once
#include "Engine/Math/Point2.hpp"

namespace Simple
{

	template<typename T>
	class Triangle2 final
	{
	public:

		constexpr Triangle2() = default;
		constexpr Triangle2(const Point2<T>& point0, const Point2<T>& point1, const Point2<T>& point2);

		[[nodiscard]] constexpr Point2<T>& GetPoint0() noexcept;
		[[nodiscard]] constexpr Point2<T>& GetPoint1() noexcept;
		[[nodiscard]] constexpr Point2<T>& GetPoint2() noexcept;

		[[nodiscard]] constexpr const Point2<T>& GetPoint0() const noexcept;
		[[nodiscard]] constexpr const Point2<T>& GetPoint1() const noexcept;
		[[nodiscard]] constexpr const Point2<T>& GetPoint2() const noexcept;

	private:

		Point2<T> mPoint0;
		Point2<T> mPoint1;
		Point2<T> mPoint2;
	};

	using Triangle2f = Triangle2<float>;
	using Triangle2d = Triangle2<double>;

	template<typename T>
	constexpr Triangle2<T>::Triangle2(const Point2<T>& point0, const Point2<T>& point1, const Point2<T>& point2)
		: mPoint0(point0)
		, mPoint1(point1)
		, mPoint2(point2)
	{
	}

	template<typename T>
	constexpr Point2<T>& Triangle2<T>::GetPoint0() noexcept
	{
		return mPoint0;
	}

	template<typename T>
	constexpr Point2<T>& Triangle2<T>::GetPoint1() noexcept
	{
		return mPoint1;
	}

	template<typename T>
	constexpr Point2<T>& Triangle2<T>::GetPoint2() noexcept
	{
		return mPoint2;
	}

	template<typename T>
	constexpr const Point2<T>& Triangle2<T>::GetPoint0() const noexcept
	{
		return mPoint0;
	}

	template<typename T>
	constexpr const Point2<T>& Triangle2<T>::GetPoint1() const noexcept
	{
		return mPoint1;
	}

	template<typename T>
	constexpr const Point2<T>& Triangle2<T>::GetPoint2() const noexcept
	{
		return mPoint2;
	}
}