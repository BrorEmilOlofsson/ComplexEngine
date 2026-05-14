#pragma once
#include "Engine/ECS/ECS.hpp"
#include "Engine/Utility/Blackboard.hpp"

namespace CLX
{
    
    class TriggerSystem
    {
    public:

        static void Update(ECS& ecs, const float deltaTime, const Blackboard& blackboard);

        static void Render(const ECS& ecs, const Blackboard& blackboard);
    };
}