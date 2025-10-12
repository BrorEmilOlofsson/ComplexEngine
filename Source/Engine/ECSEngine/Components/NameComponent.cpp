#include "Engine/Precompiled/EnginePch.hpp"
#include "NameComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace Simple
{

	void RegisterNameComponent()
	{
	}

	COMPONENT(NameComponent, IsDefaultComponent{});

	REGISTER_MEMBER(NameComponent::name);

}