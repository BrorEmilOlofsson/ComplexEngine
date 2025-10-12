#pragma once
#include <iostream>
#include "Utility/Math/Math.hpp"

namespace Simple
{
	template<typename T>
	class Vector3 final
	{
	public:
		T x = T{};
		T y = T{};
		T z = T{};

		constexpr Vector3() noexcept = default;
		constexpr Vector3(const T& x, const T& y, const T& z) noexcept;
		constexpr explicit Vector3(const T& value) noexcept;
		template<typename U>
		constexpr explicit Vector3(const Vector3<U>& vector) noexcept;

		[[nodiscard]] static constexpr Vector3<T> Zero() noexcept;
		[[nodiscard]] static constexpr Vector3<T> One() noexcept;

		using value_type = T;
	};

	using Vector3i = Vector3<int>;
	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;
	using Vector3ui = Vector3<unsigned int>;

	template<typename T>
	constexpr Vector3<T>::Vector3(const T& x, const T& y, const T& z) noexcept
		: x(x)
		, y(y)
		, z(z)
	{
	}

	template<typename T>
	constexpr Vector3<T>::Vector3(const T& value) noexcept
		: x(value)
		, y(value)
		, z(value)
	{
	}

	template<typename T>
	template<typename U>
	constexpr Vector3<T>::Vector3(const Vector3<U>& vector) noexcept
		: x(static_cast<T>(vector.x))
		, y(static_cast<T>(vector.y))
		, z(static_cast<T>(vector.z))
	{
	}

	template<typename T>
	constexpr Vector3<T> Vector3<T>::Zero() noexcept
	{
		return Vector3<T>(0, 0, 0);
	}

	template<typename T>
	constexpr Vector3<T> Vector3<T>::One() noexcept
	{
		return Vector3<T>(1, 1, 1);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b) noexcept
	{
		return Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b) noexcept
	{
		return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator*(const Vector3<T>& a, const Vector3<T>& b) noexcept
	{
		return Vector3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator*(const Vector3<T>& vector, const T& scalar) noexcept
	{
		return Vector3<T>(vector.x * scalar, vector.y * scalar, vector.z * scalar);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator*(const T& scalar, const Vector3<T>& vector) noexcept
	{
		return vector * scalar;
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator/(const Vector3<T>& vector, const T& scalar)
	{
		return Vector3<T>(vector.x / scalar, vector.y / scalar, vector.z / scalar);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator/(const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector3<T>(a.x / b.x, a.y / b.y, a.z / b.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator/(const T& scalar, const Vector3<T>& vector)
	{
		return Vector3<T>(scalar / vector.x, scalar / vector.y, scalar / vector.z);
	}

	template<typename T>
	constexpr Vector3<T>& operator+=(Vector3<T>& a, const Vector3<T>& b) noexcept
	{
		a = a + b;
		return a;
	}

	template<typename T>
	constexpr Vector3<T>& operator-=(Vector3<T>& a, const Vector3<T>& b) noexcept
	{
		a = a + b;
		return a;
	}

	template<typename T>
	constexpr Vector3<T>& operator*=(Vector3<T>& vector, const T& scalar) noexcept
	{
		vector = vector * scalar;
		return vector;
	}

	template<typename T>
	constexpr Vector3<T>& operator/=(Vector3<T>& vector, const T& scalar)
	{
		vector = vector / scalar;
		return vector;
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator-(const Vector3<T>& vector) noexcept
	{
		return Vector3<T>(-vector.x, -vector.y, -vector.z);
	}

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const Vector3<T>& a, const Vector3<T>& b) noexcept
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Vector3<T>& vector)
	{
		return os << "{ X: " << vector.x << " Y: " << vector.y << " Z: " << vector.z << " }";
	}

	template<typename T>
	[[nodiscard]] constexpr bool NearlyEqual(const Vector3<T>& a, const Vector3<T>& b, const T& tolerance = static_cast<T>(0.0001)) noexcept
	{
		return (Abs(a.x - b.x) < tolerance) && (Abs(a.y - b.y) < tolerance) && (Abs(a.z - b.z) < tolerance);
	}
}