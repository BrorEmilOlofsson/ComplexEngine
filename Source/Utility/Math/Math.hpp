#pragma once
#include <vector>
#include <utility>
#include <algorithm>
#include <concepts>
#include <array>
#include <cmath>
#include <optional>
#include "Utility/Math/MathConstants.hpp"
#include "Utility/Math/Angle.hpp"

#undef max
#undef min

namespace Simple
{
	template<typename T>
	[[nodiscard]] constexpr T Square(const T& value) noexcept
	{
		return value * value;
	}

	template<typename T>
	[[nodiscard]] constexpr T Sqrt(const T& value) noexcept
	{
		return std::sqrt(value);
	}

	template<typename T>
	[[nodiscard]] constexpr T Abs(const T& value) noexcept
	{
		return value < 0 ? -value : value;
	}

	template<typename T>
	[[nodiscard]] constexpr T Min(const T& a, const T& b) noexcept
	{
		return (a < b) ? a : b;
	}

	template<typename T>
	[[nodiscard]] constexpr T Max(const T& a, const T& b) noexcept
	{
		return (a > b) ? a : b;
	}

	template<typename T>
	[[nodiscard]] constexpr T Clamp(const T& value, const T& min, const T& max) noexcept
	{
		return (value < min) ? min : (value > max) ? max : value;
	}

	template<typename T>
	[[nodiscard]] constexpr T Sign(const T& value) noexcept
	{
		return value < 0 ? T{ -1 } : value == 0 ? 0 : T{ 1 };
	}

	template<typename T>
	[[nodiscard]] constexpr T BoolToSign(const bool& value) noexcept
	{
		return value ? T{ 1 } : T{ -1 };
	}

	template<std::floating_point T>
	[[nodiscard]] constexpr T Floor(const T& value) noexcept
	{
		return static_cast<float>((value < 0.0) ? (static_cast<int>(value) - (value != static_cast<int>(value))) : static_cast<int>(value));
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsInRangeInclusive(const T& value, const T& min, const T& max) noexcept
	{
		return value >= min && value <= max;
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsInRangeInclusiveExclusive(const T& value, const T& min, const T& max) noexcept
	{
		return value >= min && value < max;
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsInRangeExclusiveInclusive(const T& value, const T& min, const T& max) noexcept
	{
		return value > min && value <= max;
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsInRangeExclusive(const T& value, const T& min, const T& max) noexcept
	{
		return value > min && value < max;
	}

	template<typename T>
	[[nodiscard]] constexpr T Wrap(const T& value, const T& min, const T& max) noexcept
	{
		using SignedType = std::make_signed_t<T>;
		const SignedType diff = ((max - min) + 1) * Sign<SignedType>(max - value);
		T valueTemp = value;
		while (!IsInRangeInclusive(valueTemp, min, max))
		{
			valueTemp += diff;
		}
		return valueTemp;
	}

	template<typename T, typename U>
	[[nodiscard]] constexpr T Lerp(const T& start, const T& target, const U& percent) noexcept
	{
		return static_cast<T>(start + percent * (target - start));
	}

	template<typename T, typename U>
	[[nodiscard]] constexpr T LerpClamped(const T& start, const T& target, const U& percent) noexcept
	{
		return Lerp(start, target, std::clamp(percent, U{ 0 }, U{ 1 }));
	}

	template<typename T, typename U>
	[[nodiscard]] constexpr T SmoothStep(const T& start, const T& target, const U& percent) noexcept
	{
		const U percentClamped = std::clamp(percent, 0.0f, 1.0f);
		const U t = percentClamped * percentClamped * (3.0f - 2.0f * percentClamped);
		return Lerp(start, target, t);
	}

	template<typename T>
	[[nodiscard]] constexpr auto Remap0To1(const T& value, const T& a, const T& b) noexcept -> decltype((value - a) / (b - a))
	{
		return (value - a) / (b - a);
	}

	template<typename T>
	[[nodiscard]] constexpr T Remap(const T& value, const T& oldMin, const T& oldMax, const T& newMin, const T& newMax) noexcept
	{
		using PercentType = std::conditional_t<std::integral<T>, double, T>;
		return Lerp(newMin, newMax, Remap0To1<PercentType>(value, oldMin, oldMax));
	}

	template<typename T>
	[[nodiscard]] constexpr T Sin(const Radians<T>& angle) noexcept
	{
		return std::sin(angle.Value());
	}

	template<typename T>
	[[nodiscard]] constexpr T Sin(const Degrees<T>& angle) noexcept
	{
		return Sin(ToRadians(angle));
	}

	template<typename T>
	[[nodiscard]] constexpr T Cos(const Radians<T>& angle) noexcept
	{
		return std::cos(angle.Value());
	}

	template<typename T>
	[[nodiscard]] constexpr T Cos(const Degrees<T>& angle) noexcept
	{
		return Cos(ToRadians(angle));
	}

	template<typename T>
	[[nodiscard]] constexpr T Tan(const Radians<T>& angle) noexcept
	{
		return std::tan(angle.Value());
	}

	template<typename T>
	[[nodiscard]] constexpr T Tan(const Degrees<T>& angle) noexcept
	{
		return Tan(ToRadians(angle));
	}

	template<typename T>
	[[nodiscard]] constexpr Radians<T> ASin(const T& value) noexcept
	{
		return Radians<T>(std::asin(value));
	}

	template<typename T>
	[[nodiscard]] constexpr Radians<T> ACos(const T& value) noexcept
	{
		return Radians<T>(std::acos(value));
	}

	template<typename T>
	[[nodiscard]] constexpr Radians<T> ATan(const T& value) noexcept
	{
		return Radians<T>(std::atan(value));
	}

	template<typename T>
	[[nodiscard]] constexpr Radians<T> ATan2(const T& y, const T& x) noexcept
	{
		return Radians<T>(std::atan2(y, x));
	}

	enum class eQuadraticReturn : uint8_t
	{
		Min,
		Max,
		Both
	};

	template<std::floating_point T, eQuadraticReturn Return = eQuadraticReturn::Both>
	[[nodiscard]] constexpr auto Quadratic(const T& a, const T& b, const T& c)
		-> std::conditional_t<Return == eQuadraticReturn::Both, std::optional<std::pair<T, T>>, std::optional<T>>
	{
		const T discriminant = b * b - 4 * a * c;
		if (discriminant < 0)
		{
			return std::nullopt;
		}
		const T sqrtDiscriminant = std::sqrt(discriminant);

		const std::pair<T, T> result
		{
			(-b + sqrtDiscriminant) / (2 * a),
			(-b - sqrtDiscriminant) / (2 * a)
		};
		if constexpr (Return == eQuadraticReturn::Min)
		{
			return Min(result.first, result.second);
		}
		else if constexpr (Return == eQuadraticReturn::Max)
		{
			return Max(result.first, result.second);
		}
		else
		{
			return result;
		}
	}

	template<typename T>
	[[nodiscard]] constexpr T LengthSquared(const T& x, const T& y) noexcept
	{
		return Square(x) + Square(y);
	}

	template<typename T>
	[[nodiscard]] constexpr T Length(const T& x, const T& y) noexcept
	{
		return Sqrt(LengthSquared(x, y));
	}

	template<typename T>
	[[nodiscard]] constexpr T LengthSquared(const T& x, const T& y, const T& z) noexcept
	{
		return Square(x) + Square(y) + Square(z);
	}

	template<typename T>
	[[nodiscard]] constexpr T Length(const T& x, const T& y, const T& z) noexcept
	{
		return Sqrt(LengthSquared(x, y, z));
	}

	template<typename T>
	[[nodiscard]] constexpr T LengthSquared(const T& x, const T& y, const T& z, const T& w) noexcept
	{
		return Square(x) + Square(y) + Square(z) + Square(w);
	}

	template<typename T>
	[[nodiscard]] constexpr T Length(const T& x, const T& y, const T& z, const T& w) noexcept
	{
		return Sqrt(LengthSquared(x, y, z, w));
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsNormalized(const T& x, const T& y) noexcept
	{
		return LengthSquared(x, y) == static_cast<T>(1.0);
	}

	template<typename T>
	constexpr void AssertNormalized(const T& x, const T& y)
	{
		if (!IsNormalized(x, y))
		{
			throw std::runtime_error("Vector is not normalized");
		}
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsNormalized(const T& x, const T& y, const T& z) noexcept
	{
		return LengthSquared(x, y, z) == static_cast<T>(1.0);
	}

	template<typename T>
	constexpr void AssertNormalized(const T& x, const T& y, const T& z)
	{
		if (!IsNormalized(x, y, z))
		{
			throw std::runtime_error("Vector is not normalized");
		}
	}

	template<bool ThrowError = true, typename T>
	constexpr void Normalize(T& x, T& y, T& z, T& w) noexcept(!ThrowError)
	{
		const T lengthSquared = LengthSquared(x, y, z, w);
		if (lengthSquared == T{ 0 })
		{
			if constexpr (ThrowError)
			{
				throw std::runtime_error("Cannot normalize zero vector");
			}
			else
			{
				return;
			}
		}
		else if (lengthSquared == static_cast<T>(1.0))
		{
			return;
		}
		else
		{
			const T length = Sqrt(lengthSquared);
			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}
	}

	template<bool ThrowError = true, typename T>
	constexpr void Normalize(T& x, T& y, T& z) noexcept(!ThrowError)
	{
		const T lengthSquared = LengthSquared(x, y, z);
		if (lengthSquared == T{ 0 })
		{
			if constexpr (ThrowError)
			{
				throw std::runtime_error("Cannot normalize zero vector");
			}
			else
			{
				return;
			}
		}
		else if (lengthSquared == static_cast<T>(1.0))
		{
			return;
		}
		else
		{
			const T length = Sqrt(lengthSquared);
			x /= length;
			y /= length;
			z /= length;
		}
	}

	template<bool ThrowError = true, typename T>
	constexpr void Normalize(T& x, T& y) noexcept(!ThrowError)
	{
		const T lengthSquared = LengthSquared(x, y);
		if (lengthSquared == T{ 0 })
		{
			if constexpr (ThrowError)
			{
				throw std::runtime_error("Cannot normalize zero vector");
			}
			else
			{
				return;
			}
		}
		else if (lengthSquared == static_cast<T>(1.0))
		{
			return;
		}
		else
		{
			const T length = Sqrt(lengthSquared);
			x /= length;
			y /= length;
		}
	}

	template<typename T>
	[[nodiscard]] constexpr T Dot(const T& aX, const T& aY, const T& bX, const T& bY) noexcept
	{
		return aX * bX + aY * bY;
	}

	template<typename T>
	[[nodiscard]] constexpr T Dot(const T& aX, const T& aY, const T& aZ, const T& bX, const T& bY, const T& bZ) noexcept
	{
		return aX * bX + aY * bY + aZ * bZ;
	}

	template<typename T>
	[[nodiscard]] constexpr T Cross(const T& aX, const T& aY, const T& bX, const T& bY) noexcept
	{
		return aX * bY - aY * bX;
	}

	template<typename T>
	[[nodiscard]] constexpr std::array<T, 3> Cross(const T& aX, const T& aY, const T& aZ, const T& bX, const T& bY, const T& bZ) noexcept
	{
		return
		{
			(aY * bZ) - (aZ * bY),
			(aZ * bX) - (aX * bZ),
			(aX * bY) - (aY * bX)
		};
	}

}