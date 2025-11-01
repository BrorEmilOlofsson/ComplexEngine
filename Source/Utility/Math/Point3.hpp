#pragma once
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/UnitVector3.hpp"
#include <iostream>
#include <format>

namespace Simple
{
	template<typename T>
	class Point3 final
	{
	public:

		using value_type = T;

		constexpr Point3() noexcept = default;
		constexpr Point3(const T& x, const T& y, const T& z) noexcept;

		template<typename U>
		constexpr explicit Point3(const Point3<U>& point) noexcept;

		[[nodiscard]] static constexpr Point3<T> Zero() noexcept;

	public:

		T x = T{};
		T y = T{};
		T z = T{};
	};

	using Point3f = Point3<float>;
	using Point3d = Point3<double>;
	using Point3i = Point3<int>;
	using Point3ui = Point3<unsigned int>;

	template<typename T>
	constexpr Point3<T>::Point3(const T& x, const T& y, const T& z) noexcept
		: x(x)
		, y(y)
		, z(z)
	{
	}

	template<typename T>
	template<typename U>
	constexpr Point3<T>::Point3(const Point3<U>& point) noexcept
		: x(static_cast<T>(point.x))
		, y(static_cast<T>(point.y))
		, z(static_cast<T>(point.z))
	{
	}

	template<typename T>
	constexpr Point3<T> Point3<T>::Zero() noexcept
	{
		return Point3<T>(0, 0, 0);
	}

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const Point3<T>& a, const Point3<T>& b) noexcept
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> operator+(const Point3<T>& point, const Vector3<T>& vector) noexcept
	{
		return Point3<T>(point.x + vector.x, point.y + vector.y, point.z + vector.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> operator+(const Point3<T>& point, const UnitVector3<T>& vector) noexcept
	{
		return Point3<T>(point.x + vector.X(), point.y + vector.Y(), point.z + vector.Z());
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> operator-(const Point3<T>& point, const Vector3<T>& vector) noexcept
	{
		return Point3<T>(point.x - vector.x, point.y - vector.y, point.z - vector.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator-(const Point3<T>& a, const Point3<T>& b) noexcept
	{
		return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	template<typename T>
	constexpr Point3<T>& operator+=(Point3<T>& point, const Vector3<T>& vector) noexcept
	{
		point = point + vector;
		return point;
	}

	template<typename T>
	constexpr Point3<T>& operator-=(Point3<T>& point, const Vector3<T>& vector) noexcept
	{
		point = point - vector;
		return point;
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Point3<T>& point)
	{
		return os << "Point3(" << point.x << ", " << point.y << ", " << point.z << ")";
	}

	template<typename T>
	[[nodiscard]] constexpr bool NearlyEqual(const Point3<T>& a, const Point3<T>& b, const T& tolerance = static_cast<T>(0.0001)) noexcept
	{
		return (Abs(a.x - b.x) < tolerance) && (Abs(a.y - b.y) < tolerance) && (Abs(a.z - b.z) < tolerance);
	}
}

template<typename T>
struct std::formatter<Simple::Point3<T>, char> : std::formatter<std::string, char>
{
	template<typename FormatContext>
	auto format(const Simple::Point3<T>& point, FormatContext& ctx) const
	{
		return std::format_to(ctx.out(), "{{ X: {} Y: {} Z: {} }}", point.x, point.y, point.z);
	}
};