#include "Engine/Precompiled/EnginePch.hpp"
#include "RotatingMovementComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace CLX
{
	void RegisterRotatingMovementComponent() {}

	REGISTER_COMPONENT(RotatingMovementComponent);

	REGISTER_MEMBER(RotatingMovementComponent::rotationAxis);
	REGISTER_MEMBER(RotatingMovementComponent::rotationSpeed);
	REGISTER_MEMBER(RotatingMovementComponent::transformMode);
}