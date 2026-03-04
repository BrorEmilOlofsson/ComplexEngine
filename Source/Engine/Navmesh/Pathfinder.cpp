#include "Engine/Precompiled/EnginePch.hpp"
#include "Pathfinder.hpp"
#include "Navmesh.hpp"
#include "Funneling.hpp"
#include "NavmeshAStar.hpp"

#include <optional>

namespace CLX
{

	std::vector<Point3f> Pathfinder::FindPath(const Navmesh& navmesh, const Point3f& startPosition, const NavmeshPositionData& endData)
	{
		if (endData.mNodeIndex == -1 || navmesh.mNodes[endData.mNodeIndex].mState == eNavmeshNodeState::Unwalkable)
		{
			return std::vector<Point3f>();
		}
		const NavmeshPositionData startData = navmesh.GetPositionData(startPosition);
		if (startData.mNodeIndex == -1)
		{
			return std::vector<Point3f>();
		}

		return FindPath(navmesh, startData, endData);
	}

	std::vector<Point3f> Pathfinder::FindPath(const Navmesh& navmesh, const Point3f& startPosition, const Point3f& endPosition)
	{
		const NavmeshPositionData startData = navmesh.GetPositionData(startPosition);
		const NavmeshPositionData endData = navmesh.GetPositionData(endPosition);
		return FindPath(navmesh, startData, endData);
	}

	std::vector<Point3f> Pathfinder::FindPath(const Navmesh& navmesh, const NavmeshPositionData& startData, const NavmeshPositionData& endData)
	{
		PROFILER_FUNCTION(profiler::colors::Purple300);
		if (startData.mNodeIndex == -1 || endData.mNodeIndex == -1 ||
			navmesh.mNodes[startData.mNodeIndex].mState == eNavmeshNodeState::Unwalkable || navmesh.mNodes[endData.mNodeIndex].mState == eNavmeshNodeState::Unwalkable)
		{
			return std::vector<Point3f>();
		}

		if (startData.mNodeIndex == endData.mNodeIndex)
		{
			std::vector<Point3f> path;

			path.push_back(endData.m3DPosition);
			return path;
		}

		std::vector<NavmeshNodeIndex> aStarPath = NavmeshAStar::FindPath(navmesh, startData.mNodeIndex, endData.mNodeIndex);
		if (aStarPath.empty())
		{
			return std::vector<Point3f>();
		}

		const std::vector<NavmeshVertexIndex> vertexPath = Funneling::FindPath(navmesh, startData, endData, aStarPath);

		const size_t vertexPathSize = vertexPath.size();
		const size_t pathSize = vertexPathSize + 1ull;
		std::vector<Point3f> path(pathSize);

		for (size_t i = 0; i < vertexPathSize; ++i)
		{
			const NavmeshVertexIndex vertexIndex = vertexPath[i];
			path[i] = navmesh.GetNavmeshData().m3DVertices[vertexIndex] + navmesh.GetNavmeshData().mPathOffsetDirections[vertexIndex] * Navmesh::Path_Offset;
		}

		path[vertexPathSize] = endData.m3DPosition;


		return path;
	}
}