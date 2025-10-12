#pragma once
#include "Vector2.hpp"

namespace Simple
{
	template<typename T>
	class Point2 final
	{
	public:

		using value_type = T;

		constexpr Point2() noexcept = default;
		constexpr Point2(const T& x, const T& y) noexcept;

		template<typename U>
		constexpr explicit Point2(const Point2<U>& point) noexcept;

		[[nodiscard]] static constexpr Point2<T> Zero() noexcept;
		[[nodiscard]] static constexpr Point2<T> One() noexcept;

	public:

		T x = T{};
		T y = T{};
	};

	using Point2f = Point2<float>;
	using Point2d = Point2<double>;
	using Point2i = Point2<int>;
	using Point2ui = Point2<unsigned int>;
	using Point2l = Point2<long>;

	template<typename T>
	constexpr Point2<T>::Point2(const T& x, const T& y) noexcept
		: x(x)
		, y(y)
	{
	}

	template<typename T>
	template<typename U>
	constexpr Point2<T>::Point2(const Point2<U>& point) noexcept
		: x(static_cast<T>(point.x))
		, y(static_cast<T>(point.y))
	{
	}

	template<typename T>
	constexpr Point2<T> Point2<T>::Zero() noexcept
	{
		return Point2<T>(0, 0);
	}

	template<typename T>
	constexpr Point2<T> Point2<T>::One() noexcept
	{
		return Point2<T>(1, 1);
	}

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const Point2<T>& a, const Point2<T>& b) noexcept
	{
		return a.x == b.x && a.y == b.y;
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> operator+(const Point2<T>& point, const Vector2<T>& vector) noexcept
	{
		return Point2<T>(point.x + vector.x, point.y + vector.y);
	}

	[[nodiscard]] constexpr Point2i operator+(const Point2i& point, const Vector2ui& vector) noexcept
	{
		return point + static_cast<Vector2i>(vector);
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> operator-(const Point2<T>& point, const Vector2<T>& vector) noexcept
	{
		return Point2<T>(point.x - vector.x, point.y - vector.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> operator-(const Point2<T>& point) noexcept
	{
		return Point2<T>(-point.x, -point.y);
	}

	template<typename T>
	constexpr Point2<T>& operator+=(Point2<T>& point, const Vector2<T>& vector) noexcept
	{
		point = point + vector;
		return point;
	}

	template<typename T>
	constexpr Point2<T>& operator-=(Point2<T>& point, const Vector2<T>& vector) noexcept
	{
		point = point - vector;
		return point;
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator-(const Point2<T>& a, const Point2<T>& b) noexcept
	{
		return Vector2<T>(a.x - b.x, a.y - b.y);
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Point2<T>& point)
	{
		os << "{ x: " << point.x << ", y: " << point.y << " }";
		return os;
	}
}