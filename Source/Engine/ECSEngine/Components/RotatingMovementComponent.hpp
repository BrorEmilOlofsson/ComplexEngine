#pragma once
#include "Engine/Math/UnitVector3.hpp"
#include "Engine/Math/TransformMode.hpp"

namespace CLX
{

	struct RotatingMovementComponent final
	{
		UnitVector3f rotationAxis;
		float rotationSpeed = 10.f;
		eTransformMode transformMode = eTransformMode::World;

	};

	void RegisterRotatingMovementComponent();
}