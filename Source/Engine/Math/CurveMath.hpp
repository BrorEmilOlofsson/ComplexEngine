#pragma once
#include "Engine/Math/Point2.hpp"
#include <span>

namespace Simple
{

    template<typename T, typename U>
    [[nodiscard]] constexpr Point2<T> QuadraticCurve(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c, const U& t) noexcept
    {
        const Point2<T> ab = Lerp(a, b, t);
        const Point2<T> bc = Lerp(b, c, t);
        return Lerp(ab, bc, t);
    }

    template<typename T, typename U>
    [[nodiscard]] constexpr Point2<T> CubicCurve(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c, const Point2<T>& d, const U& t) noexcept
    {
        const Point2<T> ab_bc = QuadraticCurve(a, b, c, t);
        const Point2<T> bc_cd = QuadraticCurve(b, c, d, t);
        return Lerp(ab_bc, bc_cd, t);
    }

    [[nodiscard]] constexpr bool PointsAreMonotonicIncreasingX(const std::ranges::range auto& points)
    {
        const std::size_t size = points.size();
        if (size < 2)
        {
            return true;
        }
        for (std::size_t i = 0; i < size - 1; ++i)
        {
            if (points[i].x >= points[i + 1].x)
            {
                return false;
            }
        }
        return true;
    }

    template<typename T>
    [[nodiscard]] constexpr T GetBezierCubicY(const std::ranges::range auto& points, const T& x)
    {
        ASSERT(PointsAreMonotonicIncreasingX(points));
        //using T = typename std::decay_t<decltype(points)>::value_type::value_type;
        const std::size_t size = points.size();
        ASSERT(size % 3 == 1);
        if (size < 2)
        {
            return T{};
        }
        for (std::size_t i = 0; i < size - 1; i += 3)
        {
            const auto& p0 = points[i];
            const auto& p3 = points[i + 3];
            if (IsInRangeInclusive(x, p0.x, p3.x))
            {
                const T t = (x - p0.x) / (p3.x - p0.x);
                const auto& p1 = points[i + 1];
                const auto& p2 = points[i + 2];
                const Point2<T> pointOnCurve = CubicCurve(p0, p1, p2, p3, t);
                return pointOnCurve.y;
            }
        }
        return T{};
    }

}