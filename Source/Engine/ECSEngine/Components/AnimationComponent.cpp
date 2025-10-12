#include "Engine/Precompiled/EnginePch.hpp"
#include "AnimationComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace Simple
{

	void RegisterAnimationComponent()
	{
	}

	COMPONENT(AnimationComponent);

	REGISTER_MEMBER(AnimationComponent::animation);
	REGISTER_MEMBER(AnimationComponent::skeleton);
}