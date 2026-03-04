#pragma once
#include "Engine/Math/UnitVector3.hpp"
#include "Engine/Math/Shapes/Sphere.hpp"
#include "Engine/Utility/Height.hpp"

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
        [[nodiscard]] constexpr Point3<T> GetApex() const noexcept;
        [[nodiscard]] constexpr Point3<T> GetBaseCenter() const noexcept;

        constexpr void SetSphereCenter(const Point3<T>& center) noexcept;
        constexpr  void SetSphereRadius(const Radius<T>& radius) noexcept;
        constexpr void SetNormal(const UnitVector3<T>& normal) noexcept;
        constexpr void SetHeight(const Height<T>& height) noexcept;


    private:

        constexpr SphericalCap(const Sphere<T>& sphere, const UnitVector3<T>& normal, const Height<T> height);

    private:

        Sphere<T> mSphere;
        UnitVector3<T> mNormal = UnitVector3<T>::Up();
        Height<T> mHeight = Height<T>{};
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

    template<typename T>
    constexpr Point3<T> SphericalCap<T>::GetApex() const noexcept
    {
        return mSphere.GetCenter() + mNormal * mSphere.GetRadius().Value();
    }

    template<typename T>
    constexpr Point3<T> SphericalCap<T>::GetBaseCenter() const noexcept
    {
        return mSphere.GetCenter() + mNormal * (mSphere.GetRadius().Value() - mHeight.Value());
    }

    template<typename T>
    constexpr void SphericalCap<T>::SetSphereCenter(const Point3<T>& center) noexcept
    {
        mSphere.SetCenter(center);
    }

    template<typename T>
    constexpr void SphericalCap<T>::SetSphereRadius(const Radius<T>& radius) noexcept
    {
        mSphere.SetRadius(radius);
    }

    template<typename T>
    constexpr void SphericalCap<T>::SetNormal(const UnitVector3<T>& normal) noexcept
    {
        mNormal = normal;
    }

    template<typename T>
    constexpr void SphericalCap<T>::SetHeight(const Height<T>& height) noexcept
    {
        mHeight = height;
    }

    template<typename T>
    [[nodiscard]] constexpr bool operator==(const SphericalCap<T>& a, const SphericalCap<T>& b) noexcept
    {
        return a.GetSphereCenter() == b.GetSphereCenter()
            && a.GetSphereRadius() == b.GetSphereRadius()
            && a.GetNormal() == b.GetNormal()
            && a.GetHeight() == b.GetHeight();
    }
}