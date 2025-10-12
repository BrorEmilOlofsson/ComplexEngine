#include "Engine/Precompiled/EnginePch.hpp"
#include "CameraComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace Simple
{
	void RegisterCameraComponent() {}


	COMPONENT(CameraComponent);

	REGISTER_MEMBER(CameraComponent::camera);
	REGISTER_MEMBER(CameraComponent::freeFly);
}