#include "Engine/Precompiled/EnginePch.hpp"
#include "ArrowComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace Simple
{

	void RegisterArrowComponent()
	{
	}

	COMPONENT(ArrowComponent);
	REGISTER_MEMBER(ArrowComponent::length);
	REGISTER_MEMBER(ArrowComponent::thickness);
	REGISTER_MEMBER(ArrowComponent::color);
}