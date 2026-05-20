#pragma once
#include <type_traits>
#include <optional>
#include <concepts>

#include "Engine/Utility/Direction.hpp"

namespace CLX
{

    template<typename T, typename U>
    [[nodiscard]] constexpr U GetRow(const T& position, const U& width)
    {
        return position / width;
    }

    template<typename T, typename U>
    [[nodiscard]] constexpr U GetColumn(const T& position, const U& width)
    {
        return position % width;
    }

    template<std::integral T, std::unsigned_integral U>
    [[nodiscard]] constexpr T GetIndex(const T& row, const T& column, const U& width)
    {
        return row * width + column;
    }

    template<std::integral T>
    [[nodiscard]] constexpr T GetRightOffset()
    {
        return 1;
    }

    template<std::integral T>
    [[nodiscard]] constexpr T GetLeftOffset()
    {
        return -1;
    }

    template<std::unsigned_integral U>
    [[nodiscard]] constexpr auto GetUpOffset(const U& width)
    {
        using SignedType = std::make_signed_t<U>;
        return -static_cast<SignedType>(width);
    }

    template<std::unsigned_integral U>
    [[nodiscard]] constexpr auto GetDownOffset(const U& width)
    {
        using SignedType = std::make_signed_t<U>;
        return static_cast<SignedType>(width);
    }

    template<std::integral T>
    [[nodiscard]] constexpr T GetPositionRightUnchecked(const T& position)
    {
        return position + GetRightOffset<T>();
    }

    template<std::integral T>
    [[nodiscard]] constexpr T GetPositionLeftUnchecked(const T& position)
    {
        return position + GetLeftOffset<T>();
    }

    template<std::integral T, std::unsigned_integral U>
    [[nodiscard]] constexpr T GetPositionUpUnchecked(const T& position, const U& width)
    {
        return position + GetUpOffset(width);
    }

    template<std::integral T, std::unsigned_integral U>
    [[nodiscard]] constexpr T GetPositionDownUnchecked(const T& position, const U& width)
    {
        return position + GetDownOffset(width);
    }

    template<std::integral T, std::unsigned_integral U>
    [[nodiscard]] constexpr std::optional<T> GetPositionRight(const T& position, const U& width)
    {
        const T newPosition = GetPositionRightUnchecked(position);
        if (newPosition % width == 0)
        {
            return std::nullopt;
        }
        return newPosition;
    }

    template<std::integral T, std::unsigned_integral U>
    [[nodiscard]] constexpr std::optional<T> GetPositionLeft(const T& position, const U& width)
    {
        if (position % width == 0)
        {
            return std::nullopt;
        }
        return GetPositionLeftUnchecked(position);
    }

    template<std::integral T, std::unsigned_integral U>
    [[nodiscard]] constexpr std::optional<T> GetPositionUp(const T& position, const U& width)
    {
        const T newPosition = GetPositionUpUnchecked(position, width);
        if (newPosition < 0)
        {
            return std::nullopt;
        }
        return newPosition;
    }

    template<std::integral T, std::unsigned_integral U>
    [[nodiscard]] constexpr std::optional<T> GetPositionDown(const T& position, const U& width, const U& height)
    {
        const T newPosition = GetPositionDownUnchecked(position, width);
        if (newPosition >= static_cast<T>(width * height))
        {
            return std::nullopt;
        }
        return newPosition;
    }

    template<std::integral T, std::unsigned_integral U>
    [[nodiscard]] constexpr std::optional<T> GetPositionUpLeft(const T& position, const U& width)
    {
        const std::optional<T> newPosition = GetPositionUp(position, width);
        if (!newPosition)
        {
            return std::nullopt;
        }
        return GetPositionLeft(*newPosition, width);
    }

    template<std::integral T, std::unsigned_integral U>
    [[nodiscard]] constexpr std::optional<T> GetPositionUpRight(const T& position, const U& width)
    {
        const std::optional<T> newPosition = GetPositionUp(position, width);
        if (!newPosition)
        {
            return std::nullopt;
        }
        return GetPositionRight(*newPosition, width);
    }

    template<std::integral T, std::unsigned_integral U>
    [[nodiscard]] constexpr std::optional<T> GetPositionDownLeft(const T& position, const U& width, const U& height)
    {
        const std::optional<T> newPosition = GetPositionDown(position, width, height);
        if (!newPosition)
        {
            return std::nullopt;
        }
        return GetPositionLeft(*newPosition, width);
    }

    template<std::integral T, std::unsigned_integral U>
    [[nodiscard]] constexpr std::optional<T> GetPositionDownRight(const T& position, const U& width, const U& height)
    {
        const std::optional<T> newPosition = GetPositionDown(position, width, height);
        if (!newPosition)
        {
            return std::nullopt;
        }
        return GetPositionRight(*newPosition, width);
    }

    template<std::integral T, std::unsigned_integral U>
    [[nodiscard]] constexpr std::optional<T> GetAdjacentPosition(const T& position, eGrid2Direction direction, const U& width, const U& height)
    {
        switch (direction)
        {
        case eGrid2Direction::Up:
            return GetPositionUp(position, width);
        case eGrid2Direction::Down:
            return GetPositionDown(position, width, height);
        case eGrid2Direction::Left:
            return GetPositionLeft(position, width);
        case eGrid2Direction::Right:
            return GetPositionRight(position, width);
        default:
            return std::nullopt;
        }
    }

}