#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/UnitVector3.hpp"
#include <cstdint>
#include <format>

namespace CLX
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
		[[nodiscard]] static constexpr Point3<T> One() noexcept;

	public:

		T x = T{};
		T y = T{};
		T z = T{};
	};

	using Point3f = Point3<float>;
	using Point3d = Point3<double>;
	using Point3i32 = Point3<int32_t>;
	using Point3ui32 = Point3<uint32_t>;
    using Point3i64 = Point3<int64_t>;
    using Point3ui64 = Point3<uint64_t>;
    using Point3i = Point3i32;
    using Point3ui = Point3ui32;

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
	constexpr Point3<T> Point3<T>::One() noexcept
	{
		return Point3<T>(1, 1, 1);
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
	[[nodiscard]] constexpr Point3<T> operator-(const Point3<T>& point, const UnitVector3<T>& vector) noexcept
	{
		return Point3<T>(point.x - vector.X(), point.y - vector.Y(), point.z - vector.Z());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator-(const Point3<T>& a, const Point3<T>& b) noexcept
	{
		return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> operator-(const Point3<T>& point) noexcept
	{
		return Point3<T>(-point.x, -point.y, -point.z);
	}

	template<typename T>
	constexpr Point3<T>& operator+=(Point3<T>& point, const Vector3<T>& vector) noexcept
	{
		point.x += vector.x;
		point.y += vector.y;
		point.z += vector.z;
		return point;
	}

	template<typename T>
	constexpr Point3<T>& operator+=(Point3<T>& point, const UnitVector3<T>& vector) noexcept
	{
		point.x += vector.X();
		point.y += vector.Y();
		point.z += vector.Z();
		return point;
	}

	template<typename T>
	constexpr Point3<T>& operator-=(Point3<T>& point, const Vector3<T>& vector) noexcept
	{
		point.x -= vector.x;
		point.y -= vector.y;
		point.z -= vector.z;
		return point;
	}

	template<typename T>
	constexpr Point3<T>& operator-=(Point3<T>& point, const UnitVector3<T>& vector) noexcept
	{
		point.x -= vector.X();
		point.y -= vector.Y();
		point.z -= vector.Z();
		return point;
	}

	template<typename T>
	[[nodiscard]] constexpr bool NearlyEqual(const Point3<T>& a, const Point3<T>& b, const Tolerance<T>& tolerance = Tolerance<T>{}) noexcept
	{
		return NearlyEqual(a.x, b.x, tolerance) && NearlyEqual(a.y, b.y, tolerance) && NearlyEqual(a.z, b.z, tolerance);
	}
}

template<typename T>
struct std::formatter<CLX::Point3<T>, char> : std::formatter<std::string, char>
{
	template<typename FormatContext>
	auto format(const CLX::Point3<T>& point, FormatContext& ctx) const
	{
		return std::format_to(ctx.out(), "{{ X: {} Y: {} Z: {} }}", point.x, point.y, point.z);
	}
};