#pragma once
#include <concepts>
#include <iostream>
#include "Engine/Math/Math.hpp"
#include "Engine/Utility/ValueType.hpp"

namespace CLX
{

    template<typename T>
    concept IsFloatingPointType = requires
    {
        typename ValueType<T>::type;
        requires std::floating_point<typename ValueType<T>::type>;
    };

    template<typename T, std::floating_point U, U _Epsilon>
    struct Approximation;
    
    template<std::floating_point T, T _Epsilon>
    struct Approximation<T, T, _Epsilon> final
    {
        T value = T{};

        static constexpr T Epsilon = _Epsilon;
    };


    template<IsFloatingPointType T, std::floating_point U, U _Epsilon>
    struct Approximation<T, U, _Epsilon> final
    {
        T value = T{};
        static constexpr U Epsilon = _Epsilon;
    };

    template<IsFloatingPointType T, std::floating_point U = typename ValueType<T>::type, U Epsilon = static_cast<U>(1e-5)>
    [[nodiscard]] constexpr auto Approx(const T& value) -> Approximation<T, U, Epsilon>
    {
        return Approximation<T, U, Epsilon>{ value };
    }

    template<typename T, std::floating_point U, U Epsilon>
    [[nodiscard]] constexpr bool operator==(const T& a, const Approximation<T, U, Epsilon>& b) noexcept
    {
        return NearlyEqual(a, b.value, Tolerance(Epsilon));
    }

    template<typename T, std::floating_point U, U Epsilon>
    [[nodiscard]] constexpr bool operator==(const Approximation<T, U, Epsilon>& a, const T& b) noexcept
    {
        return b == a;
    }

    template<typename T, std::floating_point  U, T Epsilon>
    std::ostream& operator<<(std::ostream& os, const Approximation<T, U, Epsilon>& approx)
    {
        os << approx.value << " +- " << Epsilon;
        return os;
    }
}