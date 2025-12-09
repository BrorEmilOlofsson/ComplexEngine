#pragma once
#include "Utility/Math/UnitVector3.hpp"
#include "Utility/Shapes/Sphere.hpp"
#include "Utility/Height.hpp"

namespace Simple
{

    template<typename T>
    class SphericalCap final
    {
    public:

        constexpr SphericalCap() = default;

        [[nodiscard]] static constexpr SphericalCap<T> FromSphereAndNormalAndHeight(const Sphere<T>& sphere, const UnitVector3<T>& normal, const Height<T> height);

        [[nodiscard]] constexpr const Point3<T>& GetSphereCenter() const noexcept;
        [[nodiscard]] constexpr Radius<T> GetSphereRadius() const noexcept;
        [[nodiscard]] constexpr const UnitVector3<T>& GetNormal() const noexcept;
        [[nodiscard]] constexpr Height<T> GetHeight() const noexcept;

    private:

        constexpr SphericalCap(const Sphere<T>& sphere, const UnitVector3<T>& normal, const Height<T> height);

    private:

        Sphere<T> mSphere;
        UnitVector3<T> mNormal = UnitVector3<T>::Up();
        Height<T> mHeight = T{};
    };

    using SphericalCapf = SphericalCap<float>;
    using SphericalCapd = SphericalCap<double>;

    template<typename T>
    constexpr SphericalCap<T>::SphericalCap(const Sphere<T>& sphere, const UnitVector3<T>& normal, const Height<T> height)
        : mSphere(sphere)
        , mNormal(normal)
        , mHeight(height)
    {
        ASSERT(height.Value() <= sphere.GetDiameter().Value() && "Height of spherical cap cannot be greater than the diameter of the sphere");
    }

    template<typename T>
    constexpr SphericalCap<T> SphericalCap<T>::FromSphereAndNormalAndHeight(const Sphere<T>& sphere, const UnitVector3<T>& normal, const Height<T> height)
    {
        return SphericalCap<T>(sphere, normal, height);
    }

    template<typename T>
    constexpr const Point3<T>& SphericalCap<T>::GetSphereCenter() const noexcept
    {
        return mSphere.GetCenter();
    }

    template<typename T>
    constexpr Radius<T> SphericalCap<T>::GetSphereRadius() const noexcept
    {
        return mSphere.GetRadius();
    }

    template<typename T>
    constexpr const UnitVector3<T>& SphericalCap<T>::GetNormal() const noexcept
    {
        return mNormal;
    }

    template<typename T>
    constexpr Height<T> SphericalCap<T>::GetHeight() const noexcept
    {
        return mHeight;
    }
}