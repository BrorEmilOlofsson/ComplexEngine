#pragma once
#include <concepts>
#include <iostream>
#include "Utility/Math/Math.hpp"

namespace Simple
{
    template<std::floating_point T, T Epsilon>
    struct Approximation final
    {
        T value = T{};

        static constexpr T epsilon = static_cast<T>(1e-5);
    };


    template<std::floating_point T, T Epsilon = 1e-5>
    [[nodiscard]] constexpr Approximation<T, Epsilon> Approx(const T& value)
    {
        return Approximation<T, Epsilon>{ value };
    }

    template<std::floating_point T, T Epsilon>
    [[nodiscard]] constexpr bool operator==(const T& a, const Approximation<T, Epsilon>& b) noexcept
    {
        return NearlyEqual(a, b.value, Tolerance(Epsilon));
    }

    template<std::floating_point T, T Epsilon>
    [[nodiscard]] constexpr bool operator==(const Approximation<T, Epsilon>& a, const T& b) noexcept
    {
        return b == a;
    }

    template<std::floating_point T, T Epsilon>
    std::ostream& operator<<(std::ostream& os, const Approximation<T, Epsilon>& approx)
    {
        os << approx.value << " +- " << Epsilon;
        return os;
    }
}