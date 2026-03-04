#pragma once
#include "Engine/Math/Shapes/Cylinder.hpp"
#include "Engine/Utility/RGBColor.hpp"

namespace CLX
{
    struct CylinderComponent final
    {
        Cylinderf cylinder;
        RGBColor color = Colors::White;
    };

    void RegisterCylinderComponent();
}