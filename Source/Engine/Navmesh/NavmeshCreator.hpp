#pragma once
#include <vector>
#include "NavmeshData.hpp"
#include "Engine/Math/Shapes/AABB2.hpp"
#include "Engine/Utility/Grid/Grid2.hpp"

namespace CLX
{

	class Navmesh;
	
	struct NavmeshCreatorResult
	{
		std::vector<NavmeshNode> nodes;
		std::vector<NavmeshWall> walls;
        Grid2<NavmeshNodeIndex> nodeGrid;
        Grid2<NavmeshVertexIndex> vertexGrid;
        Grid2<NavmeshWallIndex> wallGrid;
        AABB2f dimensions;
	};

	class NavmeshCreator final
	{
	public:
		[[nodiscard]]static NavmeshCreatorResult CreateNavmesh(NavmeshData& data);
	};
}