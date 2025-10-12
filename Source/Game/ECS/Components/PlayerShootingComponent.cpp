#include "Game/Precompiled/GamePch.hpp"
#include "PlayerShootingComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

void RegisterPlayerShootingComponent() {}

COMPONENT(PlayerShootingComponent);

REGISTER_MEMBER(PlayerShootingComponent::shotPower);