#pragma once
#include <concepts>

namespace CLX
{

    struct IsEven
    {
        template<std::integral T>
        [[nodiscard]] constexpr bool operator()(const T& v) const noexcept
        {
            return v % 2 == 0;
        }
    };

    struct IsOdd
    {
        template<std::integral T>
        [[nodiscard]] constexpr bool operator()(const T& v) const noexcept
        {
            return v % 2 != 0;
        }
    };

}