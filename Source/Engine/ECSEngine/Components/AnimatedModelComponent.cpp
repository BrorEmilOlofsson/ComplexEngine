#include "Engine/Precompiled/EnginePch.hpp"
#include "AnimatedModelComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace Simple
{
	
	void RegisterAnimatedModelComponent() {}

	COMPONENT(AnimatedModelComponent);
	REGISTER_MEMBER(AnimatedModelComponent::modelHandle);
}