#pragma once
#include <utility>
#include <optional>
#include <cmath>
#include <concepts>

#undef min

// Euler's number
template<std::floating_point T>
constexpr T Napier = T{ 2.71828 };

template<typename T>
constexpr T Abs(const T pValue)
{
	return pValue < 0 ? -pValue : pValue;
}

template<typename T>
constexpr T Lerp(const T pA, const T pB, const T pAlpha)
{
	return pA + (pB - pA) * pAlpha;
}

template<typename T>
constexpr T InverseLerp(const T pValue, const T pA, const T pB)
{
	return (pValue - pA) / (pB - pA);
}

template<typename T>
constexpr T Remap(const T pValue, const T pOldMin, const T pOldMax, const T pNewMin, const T pNewMax)
{
	return Lerp(pNewMin, pNewMax, InverseLerp(pValue, pOldMin, pOldMax));
}

enum class eQuadraticReturn : uint8_t
{
	Min,
	Max,
	Both
};

template<std::floating_point T, eQuadraticReturn Return = eQuadraticReturn::Both>
constexpr auto Quadratic(const T& pA, const T& pB, const T& pC)
	-> std::conditional_t<Return == eQuadraticReturn::Both, std::optional<std::pair<T, T>>, std::optional<T>>
{
	const T discriminant = pB * pB - 4 * pA * pC;
	if (discriminant < 0)
	{
		return std::nullopt;
	}
	const T sqrtDiscriminant = std::sqrt(discriminant);

	const std::pair<T, T> result
	{
		(-pB + sqrtDiscriminant) / (2 * pA),
		(-pB - sqrtDiscriminant) / (2 * pA)
	};
	if constexpr (Return == eQuadraticReturn::Min)
	{
		return std::min(result.first, result.second);
	}
	else if constexpr (Return == eQuadraticReturn::Max)
	{
		return std::max(result.first, result.second);
	}
	else
	{
		return result;
	}
}