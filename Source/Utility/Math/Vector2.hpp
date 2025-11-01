#pragma once
#include <iostream>
#include <format>

namespace Simple
{
	template<typename T>
	class Vector2 final
	{
	public:
		T x = T{};
		T y = T{};

		constexpr Vector2() noexcept = default;
		constexpr Vector2(const T& x, const T& y) noexcept;
		template<typename U>
		constexpr explicit Vector2(const Vector2<U>& vector) noexcept;

		[[nodiscard]] static constexpr Vector2<T> Zero() noexcept;
		[[nodiscard]] static constexpr Vector2<T> One() noexcept;

		using value_type = T;
	};

	using Vector2f = Vector2<float>;
	using Vector2d = Vector2<double>;
	using Vector2i = Vector2<int>;
	using Vector2ui = Vector2<unsigned int>;
	using Vector2l = Vector2<long>;

	template<typename T>
	constexpr Vector2<T>::Vector2(const T& x, const T& y) noexcept
		: x(x)
		, y(y)
	{
	}

	template<typename T>
	template<typename U>
	constexpr Vector2<T>::Vector2(const Vector2<U>& vector) noexcept
		: x(static_cast<T>(vector.x))
		, y(static_cast<T>(vector.y))
	{
	}

	template<typename T>
	constexpr Vector2<T> Vector2<T>::Zero() noexcept
	{
		return Vector2<T>(0, 0);
	}

	template<typename T>
	constexpr Vector2<T> Vector2<T>::One() noexcept
	{
		return Vector2<T>(1, 1);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b) noexcept
	{
		return Vector2<T>(a.x + b.x, a.y + b.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b) noexcept
	{
		return Vector2<T>(a.x - b.x, a.y - b.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator-(const Vector2<T>& vector) noexcept
	{
		return Vector2<T>(-vector.x, -vector.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator*(const Vector2<T>& vector, const T& scalar) noexcept
	{
		return Vector2<T>(vector.x * scalar, vector.y * scalar);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator*(const T& scalar, const Vector2<T>& vector) noexcept
	{
		return vector * scalar;
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator*(const Vector2<T>& a, const Vector2<T>& b) noexcept
	{
		return Vector2<T>(a.x * b.x, a.y * b.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator/(const Vector2<T>& vector, const T& scalar)
	{
		return Vector2<T>(vector.x / scalar, vector.y / scalar);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator/(const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(a.x / b.x, a.y / b.y);
	}

	template<typename T>
	constexpr Vector2<T>& operator+=(Vector2<T>& a, const Vector2<T>& b) noexcept
	{
		a.x += b.x;
		a.y += b.y;
		return a;
	}

	template<typename T>
	constexpr Vector2<T>& operator-=(Vector2<T>& a, const Vector2<T>& b) noexcept
	{
		a.x -= b.x;
		a.y -= b.y;
		return a;
	}

	template<typename T>
	constexpr Vector2<T>& operator*=(Vector2<T>& vector, const T& scalar) noexcept
	{
		vector.x *= scalar;
		vector.y *= scalar;
		return vector;
	}

	template<typename T>
	constexpr Vector2<T>& operator/=(Vector2<T>& vector, const T& aScalar)
	{
		const T inverse = (1 / aScalar);
		vector.x *= inverse;
		vector.y *= inverse;
		return vector;
	}

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const Vector2<T>& a, const Vector2<T>& b) noexcept
	{
		return a.x == b.x && a.y == b.y;
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Vector2<T>& vector)
	{
		return os << "{ X: " << vector.x << " Y: " << vector.y << " }";
	}
}

template<typename T>
struct std::formatter<Simple::Vector2<T>, char> : std::formatter<std::string, char>
{
	template<typename FormatContext>
	auto format(const Simple::Vector2<T>& vector, FormatContext& ctx) const
	{
		return std::format_to(ctx.out(), "{{ X: {} Y: {} }}", vector.x, vector.y);
	}
};