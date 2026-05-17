#pragma once
#include <type_traits>
#include <string_view>

namespace CLX
{

    template<typename T>
    struct EnumCount;

    template<typename T> requires std::is_enum_v<T>
    struct EnumIteratorValue
    {
        std::size_t index = 0;
        T value = static_cast<T>(0);
    };

    template<typename T, typename Func> requires std::is_enum_v<T>
    constexpr void ForEachEnum(Func&& func)
    {
        for (size_t i = 0; i < EnumCount<T>::value; i++)
        {
            const T enumValue = static_cast<T>(i);
            func(EnumIteratorValue{ i, enumValue });
        }
    }

    template<typename T>
    struct IsEnumBitfield : std::false_type {};

    template<typename T>
    constexpr bool IsEnumBitfieldV = IsEnumBitfield<T>::value;

    template<typename T>
    concept EnumMaskable = std::is_enum_v<T> && std::is_unsigned_v<std::underlying_type_t<T>> && IsEnumBitfieldV<T>;

    template<EnumMaskable T>
    [[nodiscard]] constexpr T operator|(const T lhs, const T rhs) noexcept
    {
        using UnderlyingType = std::underlying_type_t<T>;
        return static_cast<T>(static_cast<UnderlyingType>(lhs) | static_cast<UnderlyingType>(rhs));
    }

    template<EnumMaskable T>
    [[nodiscard]] constexpr T operator&(const T lhs, const T rhs) noexcept
    {
        using UnderlyingType = std::underlying_type_t<T>;
        return static_cast<T>(static_cast<UnderlyingType>(lhs) & static_cast<UnderlyingType>(rhs));
    }

    template<EnumMaskable T>
    [[nodiscard]] constexpr T operator^(const T lhs, const T rhs) noexcept
    {
        using UnderlyingType = std::underlying_type_t<T>;
        return static_cast<T>(static_cast<UnderlyingType>(lhs) ^ static_cast<UnderlyingType>(rhs));
    }

    template<EnumMaskable T>
    [[nodiscard]] constexpr T operator~(const T value) noexcept
    {
        using UnderlyingType = std::underlying_type_t<T>;
        return static_cast<T>(~static_cast<UnderlyingType>(value));
    }

    template<EnumMaskable T>
    constexpr T& operator|=(T& lhs, const T rhs) noexcept
    {
        lhs = lhs | rhs;
        return lhs;
    }

    template<EnumMaskable T>
    constexpr T& operator&=(T& lhs, const T rhs) noexcept
    {
        lhs = lhs & rhs;
        return lhs;
    }

    template<EnumMaskable T>
    constexpr T& operator^=(T& lhs, const T rhs) noexcept
    {
        lhs = lhs ^ rhs;
        return lhs;
    }

    template<EnumMaskable T>
    [[nodiscard]] constexpr bool HasAnyFlag(const T value, const T flag) noexcept
    {
        return (value & flag) != static_cast<T>(0);
    }

    template<EnumMaskable T>
    [[nodiscard]] constexpr bool HasFlag(const T value, const T flag) noexcept
    {
        return (value & flag) == flag;
    }
}