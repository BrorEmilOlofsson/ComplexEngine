#pragma once

#include "Engine/ECSEngine/Systems/RenderSystem.hpp"
#include "Engine/ECSEngine/Systems/RenderLightSystem.hpp"
#include "Engine/ECSEngine/Systems/CameraSystem.hpp"
#include "Engine/ECSEngine/Systems/AnimationSystem.hpp"
#include "Engine/ECSEngine/Systems/RotatingMovementSystem.hpp"
#include "Engine/ECSEngine/Systems/NavmeshSystem.hpp"
#include "Engine/ECSEngine/Systems/DebugShapeSystem.hpp"
#include "Engine/ECSEngine/Systems/TriggerSystem.hpp"
#include "Engine/ECSEngine/Systems/AudioSystem.hpp"
#include "Engine/Utility/TypeList.hpp"

namespace CLX
{
    using EngineECSSystems = TypeList<
        RenderSystem,
        RenderLightSystem,
        CameraSystem,
        AnimationSystem,
        RotatingMovementSystem,
        NavmeshSystem,
        DebugShapeSystem,
        TriggerSystem,
        AudioSystem
    >;


    void RegisterEngineSystems(ECSRegistry& ecsRegistry)
    {

        auto registerSystems = [&ecsRegistry]<typename... Args>(TypeList<Args...>)
        {
            (ecsRegistry.RegisterSystem<Args>(), ...);
        };
        
        registerSystems(EngineECSSystems{});
    }
}
