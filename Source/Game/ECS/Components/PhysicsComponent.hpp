#pragma once
#include "Engine/Physics/PhysicsObject.hpp"
#include "Engine/Reflection/Reflection.hpp"

struct PhysicsComponent final
{
	Simple::Physics::PhysicsObject physicsObject;
};

void RegisterPhysicsComponent();