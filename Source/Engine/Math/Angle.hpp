#pragma once
#include "Engine/Utility/StrongType.hpp"
#include "Engine/Math/MathConstants.hpp"

namespace CLX
{
	
	template<typename T>
	using Radians = StrongType<T, struct STRadians>;

	using Radiansf = Radians<float>;
	using Radiansd = Radians<double>;

	template<typename T>
	using Degrees = StrongType<T, struct STDegrees>;

	using Degreesf = Degrees<float>;
	using Degreesd = Degrees<double>;

	template<typename T>
	constexpr Degrees<T> ToDegrees(const Radians<T>& radians) noexcept
	{
		return Degrees<T>(radians.Value() * RAD_TO_DEG<T>);
	}

	template<typename T>
	constexpr Radians<T> ToRadians(const Degrees<T>& degrees) noexcept
	{
		return Radians<T>(degrees.Value() * DEG_TO_RAD<T>);
	}

	constexpr Degreesf WholeRotationDeg = Degreesf(360);
	constexpr Radiansf WholeRotationRad = ToRadians(WholeRotationDeg);

	template<typename T>
	auto Add(const Radians<T>&, const Radians<T>&) -> Radians<T>;

	template<typename T>
	auto Add(const Degrees<T>&, const Degrees<T>&) -> Degrees<T>;

	template<typename T>
	auto Subtract(const Radians<T>&, const Radians<T>&) -> Radians<T>;

	template<typename T>
	auto Subtract(const Degrees<T>&, const Degrees<T>&) -> Degrees<T>;

	template<typename T>
	auto AddEqual(Radians<T>&, const Radians<T>&) -> Radians<T>&;

	template<typename T>
	auto AddEqual(Degrees<T>&, const Degrees<T>&) -> Degrees<T>&;

	template<typename T>
	auto SubtractEqual(Radians<T>&, const Radians<T>&) -> Radians<T>&;

	template<typename T>
	auto SubtractEqual(Degrees<T>&, const Degrees<T>&) -> Degrees<T>&;

	template<typename T>
	auto Multiply(const Radians<T>&, const T&) -> Radians<T>;

	template<typename T>
	auto Multiply(const Degrees<T>&, const T&) -> Degrees<T>;

	template<typename T>
	auto Divide(const Radians<T>&, const T&) -> Radians<T>;

	template<typename T>
	auto Divide(const Degrees<T>&, const T&) -> Degrees<T>;

	template<typename T>
	auto Negate(const Radians<T>&) -> Radians<T>;

	template<typename T>
	auto Equals(const Radians<T>&, const Radians<T>&) -> bool;

	template<typename T>
	auto Equals(const Degrees<T>& a, const Degrees<T>&) -> bool;

	template<typename T>
	auto Compare(const Radians<T>&, const Radians<T>&) -> bool;

	template<typename T>
	auto Compare(const Degrees<T>&, const Degrees<T>&) -> bool;

	template<typename T>
	auto Print(const Radians<T>&) -> std::ostream&;

	template<typename T>
	auto Print(const Degrees<T>&) -> std::ostream&;
}