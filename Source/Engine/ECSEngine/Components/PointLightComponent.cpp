#include "Engine/Precompiled/EnginePch.hpp"
#include "PointLightComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace CLX
{
	void RegisterPointLightComponent() {}

	REGISTER_COMPONENT(PointLightComponent);

	REGISTER_MEMBER(PointLightComponent::pointLight);
}