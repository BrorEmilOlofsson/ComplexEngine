#pragma once
#include <External/fmod/core/fmod_common.h>
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Point3.hpp"

namespace CLX
{

    [[nodiscard]] constexpr Vector3f ToVector3(FMOD_VECTOR fmodVector)
    {
        return Vector3f(fmodVector.x, fmodVector.y, fmodVector.z);
    }

    [[nodiscard]] constexpr Point3f ToPoint3(FMOD_VECTOR fmodVector)
    {
        return Point3f(fmodVector.x, fmodVector.y, fmodVector.z);
    }

    [[nodiscard]] constexpr FMOD_VECTOR ToFmodVector(const Vector3f& vector)
    {
        return FMOD_VECTOR{ vector.x, vector.y, vector.z };
    }

    [[nodiscard]] constexpr FMOD_VECTOR ToFmodVector(const Point3f& point)
    {
        return FMOD_VECTOR{ point.x, point.y, point.z };
    }
}