#pragma once
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/UnitVector2.hpp"
#include "Engine/Utility/Rebind.hpp"
#include <cstdint>
#include <format>

namespace CLX
{
    template<typename T>
    class Point2 final
    {
    public:

        using value_type = T;

        constexpr Point2() noexcept = default;
        constexpr Point2(const T& x, const T& y) noexcept;

        template<typename U>
        constexpr explicit Point2(const Point2<U>& point) noexcept;

        [[nodiscard]] static constexpr Point2<T> Zero() noexcept;
        [[nodiscard]] static constexpr Point2<T> One() noexcept;

    public:

        T x = T{};
        T y = T{};
    };

    using Point2f = Point2<float>;
    using Point2d = Point2<double>;
    using Point2i32 = Point2<int32_t>;
    using Point2ui32 = Point2<uint32_t>;
    using Point2i64 = Point2<int64_t>;
    using Point2ui64 = Point2<uint64_t>;
    using Point2i = Point2i32;
    using Point2ui = Point2ui32;

    template<typename T, typename U>
    struct rebind<Point2<T>, U>
    {
        using type = Point2<U>;
    };

    template<typename T>
    constexpr Point2<T>::Point2(const T& x, const T& y) noexcept
        : x(x)
        , y(y)
    {
    }

    template<typename T>
    template<typename U>
    constexpr Point2<T>::Point2(const Point2<U>& point) noexcept
        : x(static_cast<T>(point.x))
        , y(static_cast<T>(point.y))
    {
    }

    template<typename T>
    constexpr Point2<T> Point2<T>::Zero() noexcept
    {
        return Point2<T>(0, 0);
    }

    template<typename T>
    constexpr Point2<T> Point2<T>::One() noexcept
    {
        return Point2<T>(1, 1);
    }

    template<typename T>
    [[nodiscard]] constexpr bool operator==(const Point2<T>& a, const Point2<T>& b) noexcept
    {
        return a.x == b.x && a.y == b.y;
    }

    template<typename T, typename U>
    [[nodiscard]] constexpr auto operator+(const Point2<T>& point, const Vector2<U>& vector) noexcept -> Point2<T>
    {
        using R = T;
        return Point2<R>
            (
                static_cast<R>(point.x) + static_cast<R>(vector.x),
                static_cast<R>(point.y) + static_cast<R>(vector.y)
            );
    }

    template<typename T, typename U>
    [[nodiscard]] constexpr auto operator+(const Point2<T>& point, const UnitVector2<U>& vector) noexcept -> Point2<T>
    {
        using R = T;
        return Point2<R>
            (
                static_cast<R>(point.x) + static_cast<R>(vector.X()),
                static_cast<R>(point.y) + static_cast<R>(vector.Y())
            );
    }

    template<typename T, typename U>
    [[nodiscard]] constexpr auto operator-(const Point2<T>& point, const Vector2<U>& vector) noexcept -> Point2<T>
    {
        using R = T;
        return Point2<R>
            (
                static_cast<R>(point.x) - static_cast<R>(vector.x),
                static_cast<R>(point.y) - static_cast<R>(vector.y)
            );
    }

    template<typename T, typename U>
    [[nodiscard]] constexpr auto operator-(const Point2<T>& point, const UnitVector2<U>& vector) noexcept -> Point2<T>
    {
        using R = T;
        return Point2<R>
            (
                static_cast<R>(point.x) - static_cast<R>(vector.X()),
                static_cast<R>(point.y) - static_cast<R>(vector.Y())
            );
    }

    template<typename T>
    [[nodiscard]] constexpr Point2<T> operator-(const Point2<T>& point) noexcept
    {
        return Point2<T>(-point.x, -point.y);
    }

    template<typename T, typename U>
    constexpr Point2<T>& operator+=(Point2<T>& point, const Vector2<U>& vector) noexcept 
    {
        point.x += static_cast<T>(vector.x);
        point.y += static_cast<T>(vector.y);
        return point;
    }

    template<typename T, typename U>
    constexpr Point2<T>& operator+=(Point2<T>& point, const UnitVector2<U>& vector) noexcept
    {
        point.x += static_cast<T>(vector.X());
        point.y += static_cast<T>(vector.Y());
        return point;
    }

    template<typename T, typename U>
    constexpr Point2<T>& operator-=(Point2<T>& point, const Vector2<U>& vector) noexcept
    {
        point.x -= static_cast<T>(vector.x);
        point.y -= static_cast<T>(vector.y);
        return point;
    }

    template<typename T, typename U>
    constexpr Point2<T>& operator-=(Point2<T>& point, const UnitVector2<U>& vector) noexcept
    {
        point.x -= static_cast<T>(vector.X());
        point.y -= static_cast<T>(vector.Y());
        return point;
    }

    template<typename T, typename U>
    [[nodiscard]] constexpr auto operator-(const Point2<T>& a, const Point2<U>& b) noexcept -> Vector2<std::common_type_t<T, U>>
    {
        using R = std::common_type_t<T, U>;
        return Vector2<R>
            (
                static_cast<R>(a.x) - static_cast<R>(b.x),
                static_cast<R>(a.y) - static_cast<R>(b.y)
            );
    }
}

template<typename T>
struct std::formatter<CLX::Point2<T>> : std::formatter<std::string>
{
    template<typename FormatContext>
    auto format(const CLX::Point2<T>& point, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "{{ x: {}, y: {} }}", point.x, point.y);
    }
};