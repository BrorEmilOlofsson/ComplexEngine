#pragma once
#include "Utility/Math/Point2.hpp"

namespace Simple
{

    template<typename T>
    [[nodiscard]] constexpr Point2<T> QuadraticCurve(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c, const T& t) noexcept
    {
        const Point2<T> ab = Lerp(a, b, t);
        const Point2<T> bc = Lerp(b, c, t);
        return Lerp(ab, bc, t);
    }

    template<typename T>
    [[nodiscard]] constexpr Point2<T> CubicCurve(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c, const Point2<T>& d, const T& t) noexcept
    {
        const Point2<T> ab_bc = QuadraticCurve(a, b, c, t);
        const Point2<T> bc_cd = QuadraticCurve(b, c, d, t);
        return Lerp(ab_bc, bc_cd, t);
    }

}