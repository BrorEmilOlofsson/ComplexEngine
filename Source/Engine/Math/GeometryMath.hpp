#pragma once
#include "Engine/Math/MathConstants.hpp"
#include "Engine/Math/Math.hpp"
#include "Engine/Utility/StrongType.hpp"
#include "Engine/Utility/Clamper.hpp"
#include "Engine/Utility/ValueType.hpp"
#include <type_traits>

namespace Simple
{

	template<typename T>
	using Radius = StrongAssertedTypeNew<T, PositiveOrZeroAsserter<T>, struct ST_Radius, EquatableTrait, ComparableTrait, ScalarMultiplicableTrait>;

	using Radiusf = Radius<float>;
	using Radiusd = Radius<double>;

	template<typename T>
    struct ValueType<Radius<T>> : std::type_identity<T> {};

	template<typename T>
	using Diameter = StrongAssertedTypeNew<T, PositiveOrZeroAsserter<T>, struct ST_Diameter, EquatableTrait, ComparableTrait>;

	using Diameterf = Diameter<float>;
	using Diameterd = Diameter<double>;

	template<typename T>
	using Circumference = StrongAssertedTypeNew<T, PositiveOrZeroAsserter<T>, struct ST_Circumference, EquatableTrait, ComparableTrait, ScalarMultiplicableTrait>;

	using Circumferencef = Circumference<float>;
	using Circumferenced = Circumference<double>;

	template<typename T>
	constexpr Radius<T> ToRadius(const Circumference<T>& circumference)
	{
		return Radius<T>(circumference.Value() / (2 * PI<T>));
	}

	template<typename T>
	constexpr Radius<T> ToRadius(const Diameter<T>& diameter)
	{
		return Radius<T>(diameter.Value() * 0.5f);
	}

	template<typename T>
	constexpr Diameter<T> ToDiameter(const Radius<T>& radius)
	{
		return Diameter<T>(radius.Value() * 2);
	}

	template<typename T>
	constexpr Circumference<T> ToCircumference(const Radius<T>& radius)
	{
		return Circumference<T>(radius.Value() * 2 * PI<T>);
	}

	template<typename T>
	constexpr auto Add(const Radius<T>& a, const Radius<T>& b) -> T;
}