#pragma once
#include "Engine/Utility/Blackboard.hpp"

namespace CLX
{

    class ECS;

    class DebugShapeSystem
    {
    public:

        static void Render(const ECS& ecs, const Blackboard& blackboard);

    };

}