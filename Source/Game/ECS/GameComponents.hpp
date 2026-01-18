#pragma once
#include "Game/ECS/Components/PlayerMovementComponent.hpp"
#include "Game/ECS/Components/PlanetComponent.hpp"

inline void RegisterGameComponents()
{
	RegisterPlayerMovementComponent();
	RegisterPlanetComponent();
}