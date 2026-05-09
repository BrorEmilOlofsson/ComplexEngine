#pragma once
#include <type_traits>
#include <string_view>
#include <cstdint>
#include "Engine/Utility/EnumUtility.hpp"
#include "Engine/Utility/Assert.hpp"

namespace CLX
{

    enum class eGrid2Direction : uint8_t
    {
        Up = 1 << 0,
        Down = 1 << 1,
        Left = 1 << 2,
        Right = 1 << 3,
    };

    template<>
    struct EnumCount<eGrid2Direction> : std::integral_constant<size_t, 4> {};

    template<>
    struct IsEnumBitfield<eGrid2Direction> : std::true_type {};

    [[nodiscard]] constexpr std::string_view ToString(const eGrid2Direction direction) noexcept
    {
        switch (direction)
        {
        case eGrid2Direction::Up:
            return "Up";
        case eGrid2Direction::Down:
            return "Down";
        case eGrid2Direction::Left:
            return "Left";
        case eGrid2Direction::Right:
            return "Right";
        default:
            ASSERT(false && "Invalid eGrid2Direction value.");
            return "";
        }
    }

    enum class eGrid2HorizontalDirection : uint16_t
    {
        Up = 1 << 0,
        UpRight = 1 << 1,
        Right = 1 << 2,
        DownRight = 1 << 3,
        Down = 1 << 4,
        DownLeft = 1 << 5,
        Left = 1 << 6,
        UpLeft = 1 << 7
    };

    template<>
    struct EnumCount<eGrid2HorizontalDirection> : std::integral_constant<size_t, 8> {};

    template<>
    struct IsEnumBitfield<eGrid2HorizontalDirection> : std::true_type {};

    [[nodiscard]] constexpr std::string_view ToString(const eGrid2HorizontalDirection direction) noexcept
    {
        switch (direction)
        {
        case eGrid2HorizontalDirection::Up:
            return "Up";
        case eGrid2HorizontalDirection::UpRight:
            return "UpRight";
        case eGrid2HorizontalDirection::Right:
            return "Right";
        case eGrid2HorizontalDirection::DownRight:
            return "DownRight";
        case eGrid2HorizontalDirection::Down:
            return "Down";
        case eGrid2HorizontalDirection::DownLeft:
            return "DownLeft";
        case eGrid2HorizontalDirection::Left:
            return "Left";
        case eGrid2HorizontalDirection::UpLeft:
            return "UpLeft";
        default:
            ASSERT(false && "Invalid eGrid2HorizontalDirection value.");
            return "";
        }
    }
}