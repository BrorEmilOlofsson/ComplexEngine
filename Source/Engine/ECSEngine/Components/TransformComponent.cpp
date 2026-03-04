#include "Engine/Precompiled/EnginePch.hpp"
#include "TransformComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace CLX
{

	void RegisterTransformComponent() {}


	REGISTER_COMPONENT(TransformComponent, IsDefaultComponent{});

	REGISTER_MEMBER(TransformComponent::transform);
}