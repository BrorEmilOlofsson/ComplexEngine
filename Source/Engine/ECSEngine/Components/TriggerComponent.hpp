#pragma once
#include <vector>
#include "Engine/Math/Shapes/Shape.hpp"
#include "Engine/ECS/EntityID.hpp"
#include "Engine/Utility/RGBColor.hpp"
namespace CLX
{
    
    struct TriggerComponent final
    {
        Shape shape;
        RGBColor debugColor = Colors::Green;

        std::vector<EntityID> previousCollisions;
    };
   
    void RegisterTriggerComponent();
}