#pragma once
#include "Engine/Math/Vector3.hpp"

struct PlayerMovementComponent final
{
	float walkSpeed = 5.f;
	float rotationRate = 3.f;
	CLX::Vector3f velocity;
	bool isInAir = false;
	float jumpForce = 1000.f;
	CLX::Vector3f gravityVelocity;
};


void RegisterPlayerMovementComponent();