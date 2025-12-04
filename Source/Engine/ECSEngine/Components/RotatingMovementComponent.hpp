#pragma once
#include "Utility/Math/UnitVector3.hpp"
#include "Utility/Math/TransformMode.hpp"

namespace Simple
{

	struct RotatingMovementComponent final
	{
		UnitVector3f rotationAxis;
		float rotationSpeed = 10.f;
		eTransformMode transformMode = eTransformMode::World;

	};

	void RegisterRotatingMovementComponent();
}