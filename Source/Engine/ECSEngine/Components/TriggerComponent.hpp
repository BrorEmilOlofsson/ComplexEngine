#pragma once
#include <vector>
#include "Engine/Math/Shapes/Shape.hpp"
#include "Engine/ECS/EntityID.hpp"
#include "Engine/Utility/RGBColor.hpp"

namespace CLX
{

    enum class eTriggerState : uint8_t
    {
        Enter,
        Stay,
        Exit
    };

    struct TriggerData
    {
        EntityID entityID;
        eTriggerState state = eTriggerState::Enter;
    };
    
    struct TriggerComponent final
    {
        Shape shape;
        RGBColor debugColor = Colors::Green;
        RGBColor savedDebugColor = Colors::Green;

        std::vector<TriggerData> triggers;
    };
   
    void RegisterTriggerComponent();
}