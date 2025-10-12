#pragma once
#include <cmath>
#include "Math.hpp"
#include "StrongType.hpp"

namespace Physics
{
	template<typename T>
	using Velocity = StrongType<T, struct STVelocity>;

	template<typename T>
	using Gravity = StrongType<T, struct STGravity>;

	template<typename T>
	constexpr Gravity<T> EARTH_GRAVITY = Gravity<T>{ 9.81 };

	template<typename T>
	constexpr T calculateX(const Velocity<T>& pV0, const T& pTheta, const T& pTime)
	{
		return pV0 * std::cos(pTheta) * pTime;
	}

	template<typename T>
	constexpr T calculateY(const Velocity<T>& pV0, const T& pTheta, const T& pTime, const T& pY0 = 0.0, const Gravity<T>& pGravity = EARTH_GRAVITY<T>)
	{
		return pY0 + pV0 * std::sin(pTheta) * pTime - 0.5 * pGravity * pTime * pTime;
	}

	template<typename T, eQuadraticReturn Return = eQuadraticReturn::Min>
	constexpr auto CalculateTheta(const Velocity<T>& pV0, const T& pX, const T& pY, const T& pY0 = 0.0, const Gravity<T>& pGravity = EARTH_GRAVITY<T>)
		-> std::conditional_t<Return == eQuadraticReturn::Both, std::optional<std::pair<T, T>>, std::optional<T>>
	{
		const T a = (pGravity.Value() * pX * pX) / (2 * pV0.Value() * pV0.Value());
		const T b = -pX;
		const T c = pY + a - pY0;
		if constexpr (Return == eQuadraticReturn::Both)
		{
			const std::optional<std::pair<T, T>> quadraticResult = Quadratic<T, Return>(a, b, c);
			if (!quadraticResult)
			{
				return std::nullopt;
			}
			return { std::atan(quadraticResult.first), std::atan(quadraticResult.second) };
		}
		else
		{
			const std::optional<T> quadraticResult = Quadratic<T, Return>(a, b, c);
			if (!quadraticResult)
			{
				return std::nullopt;
			}
			return std::atan(quadraticResult.value());
		}
	}
}