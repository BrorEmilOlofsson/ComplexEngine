#pragma once
#include "Utility/Math/Math.hpp"

namespace Simple
{

	template<typename T, T Min, T Max>
	using Clamper = decltype([](const T& a) -> T { return Clamp(a, Min, Max); });

	template<int Min, int Max>
	using Clamper_i = Clamper<int, Min, Max>;

	template<unsigned int Min, unsigned int Max>
	using Clamper_ui = Clamper<unsigned int, Min, Max>;

	template<unsigned long long Min, unsigned long long Max>
	using Clamper_ull = Clamper<unsigned long long, Min, Max>;

	template<float Min, float Max>
	using Clamper_f = Clamper<float, Min, Max>;

	template<double Min, double Max>
	using Clamper_d = Clamper<double, Min, Max>;


	template<typename T, T N>
	using MaxF = decltype([](const T& aValue) -> T { return Max(aValue, N); });

	template<typename T, T N>
	using MinF = decltype([](const T& aValue) -> T { return Min(aValue, N); });

	template<typename T>
	using PositiveChecker = MaxF<T, 0>;

	template<typename T>
	using NegativeChecker = MinF<T, 0>;

}