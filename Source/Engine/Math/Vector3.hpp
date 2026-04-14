#pragma once
#include <cstdint>
#include <array>
#include <type_traits>
#include "Engine/Math/Math.hpp"
#include "Engine/Utility/ValueType.hpp"
#include "Engine/Utility/Rebind.hpp"

namespace CLX
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
		constexpr explicit Vector3(const std::array<T, 3>& array) noexcept;

		template<typename U>
		constexpr explicit Vector3(const Vector3<U>& vector) noexcept;

		[[nodiscard]] static constexpr Vector3<T> Zero() noexcept;
		[[nodiscard]] static constexpr Vector3<T> One() noexcept;

		using value_type = T;
	};

	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;
	using Vector3i32 = Vector3<int32_t>;
	using Vector3ui32 = Vector3<uint32_t>;
    using Vector3i64 = Vector3<int64_t>;
    using Vector3ui64 = Vector3<uint64_t>;
    using Vector3i = Vector3i32;
    using Vector3ui = Vector3ui32;


	template<typename T, typename U>
	struct rebind<Vector3<T>, U>
	{
		using type = Vector3<U>;
	};

	template<typename T>
	struct ValueType<Vector3<T>> : std::type_identity<T> {};

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
	constexpr Vector3<T>::Vector3(const std::array<T, 3>& array) noexcept
		: x(array[0])
		, y(array[1])
		, z(array[2])
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

	template<typename T, typename U>
	[[nodiscard]] constexpr auto operator+(const Vector3<T>& a, const Vector3<U>& b) noexcept -> Vector3<std::common_type_t<T, U>>
	{
		using R =  std::common_type_t<T, U>;
		return Vector3<R>
			(
				static_cast<R>(a.x) + static_cast<R>(b.x),
				static_cast<R>(a.y) + static_cast<R>(b.y),
				static_cast<R>(a.z) + static_cast<R>(b.z)
			);
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
	constexpr Vector3<T>& operator+=(Vector3<T>& a, const Vector3<T>& b) noexcept
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
		return a;
	}

	template<typename T>
	constexpr Vector3<T>& operator-=(Vector3<T>& a, const Vector3<T>& b) noexcept
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;
		return a;
	}

	template<typename T>
	constexpr Vector3<T>& operator*=(Vector3<T>& vector, const T& scalar) noexcept
	{
		vector.x *= scalar;
		vector.y *= scalar;
		vector.z *= scalar;
		return vector;
	}

	template<typename T>
	constexpr Vector3<T>& operator*=(Vector3<T>& vector, const Vector3<T>& b)
	{
		vector.x *= b.x;
		vector.y *= b.y;
		vector.z *= b.z;
		return vector;
	}

	template<typename T>
	constexpr Vector3<T>& operator/=(Vector3<T>& vector, const Vector3<T>& b)
	{
		vector.x /= b.x;
		vector.y /= b.y;
		vector.z /= b.z;
		return vector;
	}

	template<typename T>
	constexpr Vector3<T>& operator/=(Vector3<T>& vector, const T& scalar)
	{
		const T inverse = (1 / scalar);
		vector.x *= inverse;
		vector.y *= inverse;
		vector.z *= inverse;
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
	[[nodiscard]] constexpr bool NearlyEqual(const Vector3<T>& a, const Vector3<T>& b, const Tolerance<T>& tolerance = Tolerance<T>{ static_cast<T>(0.0001) }) noexcept
	{
        return NearlyEqual(a.x, b.x, tolerance) && NearlyEqual(a.y, b.y, tolerance) && NearlyEqual(a.z, b.z, tolerance);
	}
}