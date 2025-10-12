#pragma once
#include "Game/ECS/Components/PhysicsComponent.hpp"
#include "Game/ECS/Components/BallComponent.hpp"
#include "Game/ECS/Components/PlayerShootingComponent.hpp"
#include "Game/ECS/Components/PlayerMovementComponent.hpp"
#include "Game/ECS/Components/PlanetComponent.hpp"

inline void RegisterGameComponents()
{
	RegisterPhysicsComponent();
	RegisterBallComponent();
	RegisterPlayerShootingComponent();
	RegisterPlayerMovementComponent();
	RegisterPlanetComponent();
}