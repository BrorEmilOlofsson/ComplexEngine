#pragma once
#include <concepts>
#include <ranges>
#include <type_traits>

namespace CLX
{
    template<typename T, typename ValueT>
    concept RangeOf = std::ranges::range<T> && std::same_as<std::remove_cvref_t<std::ranges::range_value_t<T>>, ValueT>;

}