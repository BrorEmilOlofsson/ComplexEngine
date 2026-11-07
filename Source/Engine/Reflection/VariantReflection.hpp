#pragma once
#include <variant>
#include <utility>
#include "Engine/Reflection/DataTypeID.hpp"

namespace CLX
{

    template<typename T, typename... Args>
    [[nodiscard]] auto GetVariantInfoImpl(auto&& variant)
    {
        using Ret = std::conditional_t<std::is_const<std::remove_reference_t<decltype(variant)>>::value, const void*, void*>;
        if (std::holds_alternative<T>(variant))
        {
            return std::pair{ GetDataTypeID<T>(), (Ret) & std::get<T>(variant)};
        }
        else if constexpr (sizeof...(Args) > 0)
        {
            return GetVariantInfoImpl<Args...>(variant);
        }
        else
        {
            ASSERT(false, "Variant type not found");
            throw std::runtime_error("Variant type not found");
        }
    }

    template<typename... Args>
    [[nodiscard]] auto GetVariantInfo(std::variant<Args...>& variant)
    {
        return GetVariantInfoImpl<Args...>(variant);
    }

    template<typename... Args>
    [[nodiscard]] auto GetVariantInfo(const std::variant<Args...>& variant)
    {
        return GetVariantInfoImpl<Args...>(variant);
    }

    template<typename T, typename... Args>
    void SetVariantTypeFilteredImpl(auto& variant, auto&& filter)
    {
        if (filter(GetDataTypeID<T>()))
        {
            variant = T{};
            return;
        }
        if constexpr (sizeof...(Args) > 0)
        {
            SetVariantTypeFilteredImpl<Args...>(variant, filter);
        }
    }

    template<typename... Args>
    void SetVariantTypeFiltered(std::variant<Args...>& variant, auto&& filter)
    {
        SetVariantTypeFilteredImpl<Args...>(variant, filter);
    }
}