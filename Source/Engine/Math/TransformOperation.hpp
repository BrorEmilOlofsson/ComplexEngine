#pragma once
#include <cstdint>
#include <type_traits>
#include "Engine/Utility/EnumUtility.hpp"

namespace CLX
{
    enum class eTransformOperation : uint32_t
    {
        None = 0,
        TranslateX = 1 << 0,
        TranslateY = 1 << 1,
        TranslateZ = 1 << 2,
        RotateX = 1 << 3,
        RotateY = 1 << 4,
        RotateZ = 1 << 5,
        ScaleX = 1 << 6,
        ScaleY = 1 << 7,
        ScaleZ = 1 << 8,
        Translate = TranslateX | TranslateY | TranslateZ,
        Rotate = RotateX | RotateY | RotateZ,
        Scale = ScaleX | ScaleY | ScaleZ,
        All = Translate | Rotate | Scale
    };

    template<>
    struct IsEnumBitfield<eTransformOperation> : std::true_type
    {};
}