#pragma once
#include "NavmeshData.hpp"
#include <queue>
#include <vector>

namespace Simple
{
	class Navmesh;

	class Pathfinder
	{
	public:

		static std::vector<Point3f> FindPath(const Navmesh& aNavmesh, const Point3f& aStartPosition, const NavmeshPositionData& aEndPosition);
		static std::vector<Point3f> FindPath(const Navmesh& aNavmesh, const Point3f& aStartPosition, const Point3f& aEndPosition);
		static std::vector<Point3f> FindPath(const Navmesh& aNavmesh, const NavmeshPositionData& aStartData, const NavmeshPositionData& aEndData);

	};
}