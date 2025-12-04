#include "Engine/Precompiled/EnginePch.hpp"
#include "DirectionalLightComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"
#include "Utility/Asset/AssetManager.hpp"

namespace Simple
{

	void RegisterDirectionalLightComponent() {}

	REGISTER_COMPONENT(DirectionalLightComponent);

	REGISTER_MEMBER(DirectionalLightComponent::directionalLight);
}