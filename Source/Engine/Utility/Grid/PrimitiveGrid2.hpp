#pragma once
#include "Engine/Math/Point2.hpp"

namespace Simple
{
	struct PrimitiveGrid2 final
	{
		Point2f minPos;
		Vector2ui gridSize;
		Vector2f cellSize;
		Vector2f offset;
	};
}