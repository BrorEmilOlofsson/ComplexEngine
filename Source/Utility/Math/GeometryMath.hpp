#pragma once
#include "Utility/Math/MathConstants.hpp"
#include "Utility/Math/Math.hpp"
#include "Utility/StrongType.hpp"
#include "Utility/Clamper.hpp"

namespace Simple
{

	template<typename T>
	using Radius = StrongCheckedType<T, PositiveChecker<T>, struct ST_Radius>;

	using Radiusf = Radius<float>;

	template<typename T>
	struct ST_Scalar<Radius<T>> : std::true_type {};

	template<typename T>
	using Diameter = StrongCheckedType<T, PositiveChecker<T>, struct ST_Diameter>;

	using Diameterf = Diameter<float>;

	template<typename T>
	struct ST_Scalar<Diameter<T>> : std::true_type {};

	template<typename T>
	using Circumference = StrongCheckedType<T, PositiveChecker<T>, struct ST_Circumference>;

	template<typename T>
	struct ST_Scalar<Circumference<T>> : std::true_type {};

	using Circumferencef = Circumference<float>;

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
}