#pragma once
#include "Engine/Math/Transform3.hpp"

namespace CLX
{

    template<typename T>
    class OBB3 final
    {
    private:

        constexpr OBB3(const Transform3<T>& transform)
            : mTransform(transform)
        {}

    public:

        constexpr OBB3() = default;

        [[nodiscard]] constexpr static OBB3<T> FromCenterAndExtents(const Point3<T>& center, const Vector3<T>& extents);
        [[nodiscard]] constexpr static OBB3<T> FromTransform(const Transform3<T>& transform);

        [[nodiscard]] constexpr Point3<T> GetCenter() const;
        [[nodiscard]] constexpr void SetCenter(const Point3<T>& center);

    private:

        Transform3<T> mTransform;
    };

    template<typename T>
    constexpr OBB3<T> OBB3<T>::FromCenterAndExtents(const Point3<T>& center, const Vector3<T>& extents)
    {
        Transform3<T> transform = Transform3<T>::FromPosition(center);
        transform.SetScale(extents * T{ 2 });
        return OBB3<T>::FromTransform(transform);
    }

    template<typename T>
    constexpr OBB3<T> OBB3<T>::FromTransform(const Transform3<T>& transform)
    {
        return OBB3<T>(transform);
    }

    template<typename T>
    constexpr Point3<T> OBB3<T>::GetCenter() const
    {
        return mTransform.GetPosition();
    }

    template<typename T>
    constexpr void OBB3<T>::SetCenter(const Point3<T>& center)
    {
        mTransform.SetPosition(center);
    }

}