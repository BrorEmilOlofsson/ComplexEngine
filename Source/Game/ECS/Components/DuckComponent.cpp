#include "Game/Precompiled/GamePch.hpp"
#include "DuckComponent.hpp"

#include "Engine/Reflection/Reflection.hpp"

void RegisterDuckComponent()
{
}

REGISTER_COMPONENT(DuckComponent)
REGISTER_MEMBER(DuckComponent::speed)
REGISTER_MEMBER(DuckComponent::ducklingIDs)