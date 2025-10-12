#pragma once
#include "Utility/StrongType.hpp"
#include "Utility/Clamper.hpp"

namespace Simple
{
	template<std::floating_point T>
	using Percent = StrongCheckedType<T, Clamper<T, 0, 1>, struct PercentTag>;

	template<std::floating_point T>
	constexpr auto Compare(const Percent<T>& a, const Percent<T>& b) -> bool;

	using Percentf = Percent<float>;
	using Percentd = Percent<double>;
}