#pragma once
#include "Utility/Math/Vector3.hpp"

struct PlayerMovementComponent final
{
	float walkSpeed = 5.f;
	float rotationRate = 3.f;
	Simple::Vector3f velocity;
	bool isInAir = false;
	float jumpForce = 1000.f;
	Simple::Vector3f gravityVelocity;
};


void RegisterPlayerMovementComponent();