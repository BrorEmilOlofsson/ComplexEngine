#include "Engine/Precompiled/EnginePch.hpp"
#include "TransformHierarchyComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace CLX
{
	void RegisterTransformHierarchyComponent() {}

	REGISTER_COMPONENT(TransformHierarchyComponent, IsDefaultComponent{});

	REGISTER_MEMBER(TransformHierarchyComponent::children);
	REGISTER_MEMBER(TransformHierarchyComponent::parent);
}