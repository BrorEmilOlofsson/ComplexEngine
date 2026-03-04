#pragma once
#include "Engine/Math/Transform3.hpp"

namespace CLX
{

	struct TransformComponent final
	{
		Transform transform;
	};

	void RegisterTransformComponent();
}