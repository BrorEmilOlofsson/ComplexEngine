#pragma once
#include "Engine/Math/Point3.hpp"
#include "Engine/Math/UnitVector3.hpp"

namespace CLX
{

    template<typename T>
    class Ray3 final
    {
    public:

        constexpr Ray3() = default;
        constexpr Ray3(const Point3<T>& origin, const UnitVector3<T>& direction);

        [[nodiscard]] static constexpr Ray3<T> FromOriginAndDirection(const Point3<T>& origin, const UnitVector3<T>& direction);
        [[nodiscard]] static constexpr Ray3<T> FromOriginAndPoint(const Point3<T>& origin, const Point3<T>& point);

        [[nodiscard]] constexpr const Point3<T>& GetOrigin() const;
        [[nodiscard]] constexpr const UnitVector3<T>& GetDirection() const;

        constexpr void SetOrigin(const Point3<T>& origin);
        constexpr void SetDirection(const UnitVector3<T>& direction);

        [[nodiscard]] constexpr Point3<T> GetPointAtDistance(const T& distance) const;

    private:

        Point3<T> mOrigin;
        UnitVector3<T> mDirection;
    };

    using Ray3f = Ray3<float>;
    using Ray3d = Ray3<double>;

    template<typename T>
    constexpr Ray3<T>::Ray3(const Point3<T>& origin, const UnitVector3<T>& direction)
        : mOrigin(origin)
        , mDirection(direction)
    {
    }

    template<typename T>
    constexpr Ray3<T> Ray3<T>::FromOriginAndDirection(const Point3<T>& origin, const UnitVector3<T>& direction)
    {
        return Ray3<T>(origin, direction);
    }

    template<typename T>
    constexpr Ray3<T> Ray3<T>::FromOriginAndPoint(const Point3<T>& origin, const Point3<T>& point)
    {
        return Ray3<T>(origin, UnitVector3<T>(point - origin));
    }

    template<typename T>
    constexpr const Point3<T>& Ray3<T>::GetOrigin() const
    {
        return mOrigin;
    }

    template<typename T>
    constexpr const UnitVector3<T>& Ray3<T>::GetDirection() const
    {
        return mDirection;
    }

    template<typename T>
    constexpr void Ray3<T>::SetOrigin(const Point3<T>& origin)
    {
        mOrigin = origin;
    }

    template<typename T>
    constexpr void Ray3<T>::SetDirection(const UnitVector3<T>& direction)
    {
        mDirection = direction;
    }

    template<typename T>
    constexpr Point3<T> Ray3<T>::GetPointAtDistance(const T& distance) const
    {
        return mOrigin + mDirection * distance;
    }

    template<typename T>
    [[nodiscard]] constexpr bool operator==(const Ray3<T>& a, const Ray3<T>& b)
    {
        return a.GetOrigin() == b.GetOrigin() && a.GetDirection() == b.GetDirection();
    }
}