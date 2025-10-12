#include "Game/Precompiled/GamePch.hpp"
#include "PlanetComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"


void RegisterPlanetComponent()
{
}

COMPONENT(PlanetComponent);
REGISTER_MEMBER(PlanetComponent::radius);
REGISTER_MEMBER(PlanetComponent::gravityMagnitude);
