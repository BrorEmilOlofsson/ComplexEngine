#pragma once
#include <variant>
#include <utility>
#include "Engine/Reflection/DataTypeID.hpp"

namespace CLX
{
    template<typename T, typename... Args>
    [[nodiscard]] std::pair<DataTypeID, void*> GetVariantInfoImpl(auto& variant)
    {
        if (std::holds_alternative<T>(variant))
        {
            return { GetDataTypeID<T>(), &std::get<T>(variant) };
        }
        else if constexpr (sizeof...(Args) > 0)
        {
            return GetVariantInfoImpl<Args...>(variant);
        }
        else
        {
            return {};
        }
    }

    template<typename... Args>
    [[nodiscard]] std::pair<DataTypeID, void*> GetVariantInfo(std::variant<Args...>& variant)
    {
        return GetVariantInfoImpl<Args...>(variant);
    }

    template<typename T, typename... Args>
    [[nodiscard]] std::pair<DataTypeID, const void*> GetVariantInfoImpl(const auto& variant)
    {
        if (std::holds_alternative<T>(variant))
        {
            return { GetDataTypeID<T>(), &std::get<T>(variant) };
        }
        else if constexpr (sizeof...(Args) > 0)
        {
            return GetVariantInfoImpl<Args...>(variant);
        }
        else
        {
            return {};
        }
    }

    template<typename... Args>
    [[nodiscard]] std::pair<DataTypeID, const void*> GetVariantInfo(const std::variant<Args...>& variant)
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