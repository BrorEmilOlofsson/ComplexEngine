#include "Engine/Precompiled/EnginePch.hpp"
#include "NameComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace CLX
{

	void RegisterNameComponent()
	{
	}

	REGISTER_COMPONENT(NameComponent, IsDefaultComponent{});

	REGISTER_MEMBER(NameComponent::name);

}