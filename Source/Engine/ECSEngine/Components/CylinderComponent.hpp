#pragma once
#include "Utility/Shapes/Cylinder.hpp"
#include "Utility/RGBColor.hpp"

namespace Simple
{
    struct CylinderComponent final
    {
        Cylinderf cylinder;
        RGBColor color = Colors::White;
    };

    void RegisterCylinderComponent();
}