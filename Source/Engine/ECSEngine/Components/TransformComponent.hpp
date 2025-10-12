#pragma once
#include "Utility/Math/Transform3.hpp"

namespace Simple
{

	struct TransformComponent final
	{
		Transform transform;
	};

	void RegisterTransformComponent();
}