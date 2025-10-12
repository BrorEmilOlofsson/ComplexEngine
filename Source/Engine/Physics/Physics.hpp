#pragma once
#include <cmath>
#include "Utility/Math/Math.hpp"
#include "Utility/Math/Point3.hpp"
#include "Utility/StrongType.hpp"
#include "Engine/Physics/Velocity.hpp"
#include "Engine/Physics/Force.hpp"
#include "Engine/Physics/Mass.hpp"
#include "Engine/Physics/Momentum.hpp"
#include "Engine/Physics/Acceleration.hpp"
#include "Engine/Physics/Gravity.hpp"

namespace Simple
{
	namespace Physics
	{

		template<typename T>
		auto Multiply(Acceleration<T>, Mass<T>) -> Force3<T>;

		template<typename T>
		auto Multiply(Mass<T>, Acceleration<T>) -> Force3<T>;

		template<typename T>
		auto Divide(Force3<T>, Mass<T>) -> Acceleration<T>;

		template<typename T>
		auto Multiply(Velocity<T>, Mass<T>) -> Momentum3<T>;

		template<typename T>
		auto Multiply(Mass<T>, Velocity<T>) -> Momentum3<T>;

		template<typename T>
		auto Divide(Momentum3<T>, Mass<T>) -> Velocity<T>;

		template<typename T>
		using Speed = StrongType<T, struct STSpeed>;

		template<typename T>
		[[nodiscard]] constexpr T GravityCalculateX(const Speed<T>& pV0, const Degrees<T>& pTheta, const T& pTime)
		{
			return pV0 * Cos(pTheta) * pTime;
		}

		template<typename T>
		[[nodiscard]] constexpr T GravityCalculateY(const Speed<T>& pV0, const Degrees<T>& pTheta, const T& pTime, const T& pY0 = 0.0, const Gravity<T>& pGravity = EARTH_GRAVITY<T>)
		{
			return pY0 + pV0 * Sin(pTheta) * pTime - 0.5 * pGravity * pTime * pTime;
		}
		

		template<typename T, eQuadraticReturn Return = eQuadraticReturn::Min>
		[[nodiscard]] constexpr auto CalculateTheta(const Speed<T>& pV0, const T& pX, const T& pY, const T& pY0 = 0.0, const Gravity<T>& pGravity = EARTH_GRAVITY<T>)
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

		template<typename T, eQuadraticReturn Return = eQuadraticReturn::Min>
		[[nodiscard]] constexpr auto CalculateTheta(const Speed<T>& pV0, const Point3<T>& pStartingPoint, const Point3<T>& pTargetPoint, const T& pY0 = 0.0, const Gravity<T>& pGravity = EARTH_GRAVITY<T>)
			-> std::conditional_t<Return == eQuadraticReturn::Both, std::optional<std::pair<T, T>>, std::optional<T>>
		{
			const Vector3<T> diff = PointDifference(pStartingPoint, pTargetPoint);
			return CalculateTheta(pV0, diff.AsVector2XZ().Length(), diff.y, pY0, pGravity);
		}


		template<typename T>
		[[nodiscard]] constexpr Velocity<T> ApplyImpulseForce(const Velocity<T>& pPreviousVelocity, const Momentum3<T>& pImpulse, const Mass<T>& pMass)
		{
			const Velocity<T> impulseVelocity = pImpulse / pMass;
			return pPreviousVelocity + impulseVelocity;
		}
	}
}