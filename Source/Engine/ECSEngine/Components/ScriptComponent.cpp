#include "Engine/Precompiled/EnginePch.hpp"
#include "ScriptComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"
#include "NodeScript/Proxy/FlyClassInstanceProxy.hpp"


namespace Fly
{
	REGISTER_DATATYPE(ClassInstanceProxy);
}

namespace Simple
{
	void RegisterScriptComponent() {}

	REGISTER_COMPONENT(ScriptComponent);

	REGISTER_MEMBER(ScriptComponent::classInstance);
}