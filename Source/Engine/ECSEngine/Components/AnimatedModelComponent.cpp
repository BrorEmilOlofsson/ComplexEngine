#include "Engine/Precompiled/EnginePch.hpp"
#include "AnimatedModelComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace CLX
{
	
	void RegisterAnimatedModelComponent() {}

	REGISTER_COMPONENT(AnimatedModelComponent);
	REGISTER_MEMBER(AnimatedModelComponent::modelHandle);
    REGISTER_MEMBER(AnimatedModelComponent::defaultAnimation);
	REGISTER_MEMBER(AnimatedModelComponent::textures);
}