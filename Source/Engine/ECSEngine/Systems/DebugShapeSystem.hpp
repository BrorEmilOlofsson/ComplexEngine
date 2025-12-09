#pragma once
#include "Utility/Blackboard.hpp"

namespace Simple
{

    class ECS;

    class DebugShapeSystem
    {
    public:

        static void Render(const ECS& ecs, const Blackboard& blackboard);

    };

}