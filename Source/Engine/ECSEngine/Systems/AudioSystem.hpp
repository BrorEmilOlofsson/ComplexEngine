#pragma once
#include "Engine/Utility/Blackboard.hpp"

namespace CLX
{

    class ECS;

    class AudioSystem final
    {
    public:

        static void Update(ECS& ecs, const float deltaTime, const Blackboard& blackboard);
        
    };
}