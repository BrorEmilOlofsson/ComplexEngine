#pragma once
#include <cstdint>
#include <format>

#include "Engine/Utility/Assert.hpp"
#include "Engine/Utility/Rebind.hpp"

namespace CLX
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
	using Vector2i32 = Vector2<int32_t>;
	using Vector2u32 = Vector2<uint32_t>;
	using Vector2i64 = Vector2<int64_t>;
	using Vector2u64 = Vector2<uint64_t>;
    using Vector2i = Vector2i32;
	using Vector2u = Vector2u32;

	template<typename T, typename U>
    struct rebind<Vector2<T>, U>
	{
		using type = Vector2<U>;
    };

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

	template<typename T, typename U>
    [[nodiscard]] constexpr auto operator*(const Vector2<T>& vector, const U& scalar) noexcept -> Vector2<std::common_type_t<T, U>>
	{
        using R = std::common_type_t<T, U>;
		return Vector2<R>
			(
				static_cast<R>(vector.x) * static_cast<R>(scalar),
				static_cast<R>(vector.y) * static_cast<R>(scalar)
			);
	}

	template<typename T, typename U>
	[[nodiscard]] constexpr auto operator*(const U& scalar, const Vector2<T>& vector) noexcept -> Vector2<std::common_type_t<T, U>>
	{
		return vector * scalar;
	}

	template<typename T, typename U>
	[[nodiscard]] constexpr auto operator*(const Vector2<T>& a, const Vector2<U>& b) noexcept -> Vector2<std::common_type_t<T, U>>
	{
		using R = std::common_type_t<T, U>;
		return 
		{
			static_cast<R>(a.x) * static_cast<R>(b.x),
			static_cast<R>(a.y) * static_cast<R>(b.y)
		};
	}

	template<typename T, typename U>
	[[nodiscard]] constexpr Vector2<T> operator/(const Vector2<T>& vector, const U& scalar)
	{
		if constexpr (std::integral<U>)
		{
			if (scalar == 0)
			{
                ASSERT(false && "Division by zero in Vector2 operator/.");
			}
		}
		const T inverse = static_cast<T>(1) / static_cast<T>(scalar);
		return Vector2<T>(vector.x * inverse, vector.y * inverse);
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
	constexpr Vector2<T>& operator*=(Vector2<T>& vector, const Vector2<T>& b) noexcept
	{
		vector.x *= b.x;
		vector.y *= b.y;
		return vector;
	}

	template<typename T>
	constexpr Vector2<T>& operator*=(Vector2<T>& vector, const T& scalar) noexcept
	{
		vector.x *= scalar;
		vector.y *= scalar;
		return vector;
	}

	template<typename T>
	constexpr Vector2<T>& operator/=(Vector2<T>& vector, const Vector2<T>& b)
	{
		vector.x /= b.x;
		vector.y /= b.y;
		return vector;
	}

	template<typename T>
	constexpr Vector2<T>& operator/=(Vector2<T>& vector, const T& scalar)
	{
		const T inverse = (1 / scalar);
		vector.x *= inverse;
		vector.y *= inverse;
		return vector;
	}

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const Vector2<T>& a, const Vector2<T>& b) noexcept
	{
		return a.x == b.x && a.y == b.y;
	}
}

template<typename T>
struct std::formatter<CLX::Vector2<T>, char> : std::formatter<std::string, char>
{
	template<typename FormatContext>
	auto format(const CLX::Vector2<T>& vector, FormatContext& ctx) const
	{
		return std::format_to(ctx.out(), "{{ X: {} Y: {} }}", vector.x, vector.y);
	}
};