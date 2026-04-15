#pragma once
#include "Engine/Math/Point2.hpp"

namespace CLX
{
	struct PrimitiveGrid2 final
	{
		Point2f minPos;
		Vector2u gridSize;
		Vector2f cellSize;
		Vector2f offset;
	};
}