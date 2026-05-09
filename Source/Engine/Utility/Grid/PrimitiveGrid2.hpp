#pragma once
#include "Engine/Math/Point2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Dimension2.hpp"

namespace CLX
{
	struct PrimitiveGrid2 final
	{
		Point2f minPos;
		Dimension2u gridSize;
		Dimension2f cellSize = Dimension2f(1, 1);
		Vector2f offset;
	};
}