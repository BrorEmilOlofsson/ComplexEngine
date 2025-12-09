#pragma once
#include "Utility/Math/Point2.hpp"
#include "Utility/Math/GeometryMath.hpp"

namespace Simple
{
    
    template<typename T>
    class Circle final
    {
    public:

        constexpr Circle() = default;

        [[nodiscard]] static constexpr Circle<T> FromCenterAndRadius(const Point2<T>& center, const Radius<T>& radius);
        [[nodiscard]] static constexpr Circle<T> FromCenter(const Point2<T>& center);
        [[nodiscard]] static constexpr Circle<T> FromRadius(const Radius<T>& radius);

        [[nodiscard]] constexpr const Point2<T>& GetCenter() const
        {
            return mCenter;
        }
        [[nodiscard]] constexpr const Radius<T>& GetRadius() const
        {
            return mRadius;
        }

        constexpr void SetCenter(const Point2<T>& center)
        {
            mCenter = center;
        }

        constexpr void SetRadius(const Radius<T>& radius)
        {
            mRadius = radius;
        }

        [[nodiscard]] constexpr Diameter<T> GetDiameter() const
        {
            return ToDiameter(mRadius);
        }

        [[nodiscard]] constexpr Circumference<T> GetCircumference() const
        {
            return ToCircumference(mRadius);
        }

    private:

        constexpr Circle(const Point2<T>& center, const Radius<T>& radius);

    private:

        Point2<T> mCenter;
        Radius<T> mRadius;
    };

    using Circlef = Circle<float>;
    using Circled = Circle<double>;

    template<typename T>
    constexpr Circle<T>::Circle(const Point2<T>& center, const Radius<T>& radius)
        : mCenter(center)
        , mRadius(radius)
    {
    }

    template<typename T>
    constexpr Circle<T> Circle<T>::FromCenterAndRadius(const Point2<T>& center, const Radius<T>& radius)
    {
        return Circle<T>(center, radius);
    }

    template<typename T>
    constexpr Circle<T> Circle<T>::FromCenter(const Point2<T>& center)
    {
        return Circle<T>(center, Radius<T>(T{ 0 }));
    }

    template<typename T>
    constexpr Circle<T> Circle<T>::FromRadius(const Radius<T>& radius)
    {
        return Circle<T>(Point2<T>::Zero(), radius);
    }
}