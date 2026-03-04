#pragma once
#include "Engine/Math/Point3.hpp"

namespace Simple
{
	struct PrimitiveGrid3 final
	{
		Point3f minPos;
		Vector3ui gridSize;
		Vector3f cellSize = Vector3f(1, 1, 1);
		Vector3f offset;
	};
}