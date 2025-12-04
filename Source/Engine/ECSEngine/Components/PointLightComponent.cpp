#include "Engine/Precompiled/EnginePch.hpp"
#include "PointLightComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace Simple
{
	void RegisterPointLightComponent() {}

	REGISTER_COMPONENT(PointLightComponent);

	REGISTER_MEMBER(PointLightComponent::pointLight);
}