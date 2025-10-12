#include "Engine/Precompiled/EnginePch.hpp"
#include "TransformHierarchyComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace Simple
{
	void RegisterTransformHierarchyComponent() {}

	COMPONENT(TransformHierarchyComponent, IsDefaultComponent{});

	REGISTER_MEMBER(TransformHierarchyComponent::children);
	REGISTER_MEMBER(TransformHierarchyComponent::parent);
}