#include "Engine/Precompiled/EnginePch.hpp"
#include "EntityCompositionInstantiationComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace CLX
{

	void RegisterEntityCompositionInstantiationComponent() {}

	REGISTER_COMPONENT(EntityCompositionInstantiationComponent, NoManualAdd{});
	REGISTER_MEMBER(EntityCompositionInstantiationComponent::asset, NoEdit{});
	REGISTER_MEMBER(EntityCompositionInstantiationComponent::mappedEntityID, NoEdit{});
}