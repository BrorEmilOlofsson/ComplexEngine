#pragma once
#include <iostream>
#include <cstdint>

namespace CLX
{
	template<typename T>
	class Vector4 final
	{
	public:
		T x = T{};
		T y = T{};
		T z = T{};
		T w = T{};

		constexpr Vector4() = default;
		constexpr Vector4(const T& x, const T& y, const T& z, const T& w) noexcept;


		[[nodiscard]] static constexpr Vector4 Zero() noexcept;
	};

	using Vector4f = Vector4<float>;
	using Vector4d = Vector4<double>;
	using Vector4i32 = Vector4<int32_t>;
	using Vector4ui32 = Vector4<uint32_t>;
	using Vector4i64 = Vector4<int64_t>;
	using Vector4ui64 = Vector4<uint64_t>;
	using Vector4i = Vector4i32;
    using Vector4ui = Vector4ui32;

	template<typename T>
	constexpr Vector4<T>::Vector4(const T& x, const T& y, const T& z, const T& w) noexcept
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{
	}

	template<typename T>
	[[nodiscard]] constexpr Vector4<T> Vector4<T>::Zero() noexcept
	{
		return Vector4<T>(0, 0, 0, 0);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector4<T> operator+(const Vector4<T>& a, const Vector4<T>& b) noexcept
	{
		return Vector4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector4<T> operator-(const Vector4<T>& a, const Vector4<T>& b) noexcept
	{
		return Vector4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector4<T> operator*(const Vector4<T>& vector, const T& scalar) noexcept
	{
		return Vector4<T>(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector4<T> operator*(const T& aScalar, const Vector4<T>& vector) noexcept
	{
		return  vector * aScalar;
	}

	template<typename T>
	[[nodiscard]] constexpr Vector4<T> operator/(const Vector4<T>& vector, const T& scalar)
	{
		return vector * (1 / scalar);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector4<T> operator/(const Vector4<T>& vector, const int& scalar)
	{
		return { vector.x / static_cast<T>(scalar), vector.y / static_cast<T>(scalar), vector.z / static_cast<T>(scalar), vector.w / static_cast<T>(scalar) };
	}

	template<typename T>
	[[nodiscard]] constexpr Vector4<T> operator/(const Vector4<T>& vector, const float& scalar)
	{
		return { vector.x / static_cast<T>(scalar), vector.y / static_cast<T>(scalar), vector.z / static_cast<T>(scalar), vector.w / static_cast<T>(scalar) };
	}

	template<typename T>
	[[nodiscard]] constexpr Vector4<T> operator/(const Vector4<T>& a, const Vector4<T>& b)
	{
		return Vector4<T>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
	}

	template<typename T>
	constexpr Vector4<T>& operator+=(Vector4<T>& a, const Vector4<T>& b) noexcept
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
		a.w += b.w;
		return a;
	}

	template<typename T>
	constexpr Vector4<T>& operator-=(Vector4<T>& a, const Vector4<T>& b) noexcept
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;
		a.w -= b.w;
		return a;
	}

	template<typename T>
	constexpr Vector4<T>& operator*=(Vector4<T>& vector, const T& scalar) noexcept
	{
		vector.x *= scalar;
		vector.y *= scalar;
		vector.z *= scalar;
		vector.w *= scalar;
		return vector;
	}

	template<typename T>
	constexpr Vector4<T>& operator/=(Vector4<T>& vector, const T& scalar)
	{
		const T inverse = (1 / scalar);
		vector.x *= inverse;
		vector.y *= inverse;
		vector.z *= inverse;
		vector.w *= inverse;
		return vector;
	}

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const Vector4<T>& a, const Vector4<T>& b) noexcept
	{
		return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Vector4<T>& vector)
	{
		return os << "{ X: " << vector.x << " Y: " << vector.y << " Z: " << vector.z << " W: " << vector.w << " }";
	}
}