#include "Game/Precompiled/GamePch.hpp"
#include "DuckComponent.hpp"

#include "Engine/Reflection/Reflection.hpp"
#include "Game/ECS/Components/DucklingComponent.hpp"
#include "Engine/ECS/EntityReferenceRequirement.hpp"

void RegisterDuckComponent()
{
}

REGISTER_COMPONENT(DuckComponent)
REGISTER_MEMBER(DuckComponent::speed)
REGISTER_MEMBER(DuckComponent::ducklingIDs)
//REGISTER_MEMBER_REQUIREMENT(DuckComponent::ducklingIDs, CLX::CreateHasComponentRequirement<DucklingComponent>())