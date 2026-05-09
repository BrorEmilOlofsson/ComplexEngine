#pragma once
#include "Engine/Math/Point3.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Dimension3.hpp"

namespace CLX
{
	struct PrimitiveGrid3 final
	{
		Point3f minPos;
		Dimension3u gridSize;
		Dimension3f cellSize = Dimension3f(1, 1, 1);
		Vector3f offset;
	};
}