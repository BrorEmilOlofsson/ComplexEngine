#pragma once
#include "Engine/Math/Math.hpp"
#include "Engine/Utility/Assert.hpp"

namespace CLX
{

    template<typename T, T Min, T Max>
    using Clamper = decltype([](const T& value) -> T { return Clamp(value, Min, Max); });

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
    using MaxF = decltype([](const T& value) -> T { return Max(value, N); });

    template<typename T, T N>
    using MinF = decltype([](const T& value) -> T { return Min(value, N); });

    template<typename T>
    using PositiveOrZeroChecker = MaxF<T, 0>;

    template<typename T>
    using NegativeOrZeroChecker = MinF<T, 0>;

    template<typename T>
    using PositiveOrZeroAsserter = decltype([](const T& value) -> void
        {
            if (value < 0)
            {
                ASSERT(false && "Value must be 0 or positive");
            }
        });

}