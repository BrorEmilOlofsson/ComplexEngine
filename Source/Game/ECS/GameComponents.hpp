#pragma once
#include "Game/ECS/Components/DuckComponent.hpp"
#include "Game/ECS/Components/DucklingComponent.hpp"

inline void RegisterGameComponents()
{
    RegisterDuckComponent();
    RegisterDucklingComponent();
}