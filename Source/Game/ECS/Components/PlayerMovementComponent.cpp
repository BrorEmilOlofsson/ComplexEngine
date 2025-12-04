#include "Game/Precompiled/GamePch.hpp"
#include "PlayerMovementComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

REGISTER_COMPONENT(PlayerMovementComponent);

REGISTER_MEMBER(PlayerMovementComponent::walkSpeed);
REGISTER_MEMBER(PlayerMovementComponent::rotationRate);
REGISTER_MEMBER(PlayerMovementComponent::velocity);
REGISTER_MEMBER(PlayerMovementComponent::isInAir);
REGISTER_MEMBER(PlayerMovementComponent::jumpForce);

void RegisterPlayerMovementComponent() {}