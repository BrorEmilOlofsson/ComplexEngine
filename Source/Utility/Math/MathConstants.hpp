#pragma once
#include <concepts>

namespace Simple
{
	template<std::floating_point T>
	constexpr T PI = static_cast<T>(3.14159265358979323846);

	// Euler's number
	template<std::floating_point T>
	constexpr T NAPIER = T{ 2.71828 };

	template<typename T>
	constexpr T DEG_TO_RAD = PI<T> / static_cast<T>(180.0);

	template<typename T>
	constexpr T RAD_TO_DEG = (static_cast<T>(180.0) / PI<T>);
}