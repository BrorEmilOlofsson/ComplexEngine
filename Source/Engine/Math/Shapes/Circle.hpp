#pragma once
#include "Engine/Math/Point2.hpp"
#include "Engine/Math/GeometryMath.hpp"

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

        [[nodiscard]] constexpr const Point2<T>& GetCenter() const;
        [[nodiscard]] constexpr const Radius<T>& GetRadius() const;

        constexpr void SetCenter(const Point2<T>& center);
        constexpr void SetRadius(const Radius<T>& radius);

        [[nodiscard]] constexpr Diameter<T> GetDiameter() const;
        [[nodiscard]] constexpr Circumference<T> GetCircumference() const;

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

    template<typename T>
    [[nodiscard]] constexpr const Point2<T>& Circle<T>::GetCenter() const
    {
        return mCenter;
    }

    template<typename T>
    [[nodiscard]] constexpr const Radius<T>& Circle<T>::GetRadius() const
    {
        return mRadius;
    }

    template<typename T>
    constexpr void Circle<T>::SetCenter(const Point2<T>& center)
    {
        mCenter = center;
    }

    template<typename T>
    constexpr void Circle<T>::SetRadius(const Radius<T>& radius)
    {
        mRadius = radius;
    }

    template<typename T>
    [[nodiscard]] constexpr Diameter<T> Circle<T>::GetDiameter() const
    {
        return ToDiameter(mRadius);
    }

    template<typename T>
    [[nodiscard]] constexpr Circumference<T> Circle<T>::GetCircumference() const
    {
        return ToCircumference(mRadius);
    }

    template<typename T>
    [[nodiscard]] constexpr bool operator==(const Circle<T>& a, const Circle<T>& b) noexcept
    {
        return (a.GetCenter() == b.GetCenter()) && (a.GetRadius() == b.GetRadius());
    }
}