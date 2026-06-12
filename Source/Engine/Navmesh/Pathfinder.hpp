#pragma once
#include "NavmeshData.hpp"
#include <queue>
#include <vector>

namespace CLX
{
	class Navmesh;

	class Pathfinder
	{
	public:

		static std::vector<Point3f> FindPath(const Navmesh& navmesh, const Point3f& startPosition, const NavmeshPositionData& endPosition);
		static std::vector<Point3f> FindPath(const Navmesh& navmesh, const Point3f& startPosition, const Point3f& endPosition);
		static std::vector<Point3f> FindPath(const Navmesh& navmesh, const NavmeshPositionData& startData, const NavmeshPositionData& endData);

	};
}