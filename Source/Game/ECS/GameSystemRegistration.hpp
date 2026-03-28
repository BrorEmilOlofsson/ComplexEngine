#pragma once
#include "Engine/Utility/TypeList.hpp"
#include "Engine/Reflection/Reflection.hpp"
#include "Game/ECS/GameComponents.hpp"

#include "Game/ECS/Systems/DuckMovementSystem.hpp"

namespace CLX
{
    constexpr TypeList<DuckMovementSystem> GameSystemTypeList{};

    struct ECSGameSystemRegistration
    {
        ECSGameSystemRegistration()
        {
            [] <typename... Systems>(TypeList<Systems...>)
            {
                ((TypeRegistration::RegisterSystem<Systems>()), ...);
            }(GameSystemTypeList);

            RegisterGameComponents();
        }
    };

    inline ECSGameSystemRegistration gameSystemRegistration;
}