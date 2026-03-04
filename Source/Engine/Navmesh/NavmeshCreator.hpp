#pragma once
#include <span>
#include "NavmeshData.hpp"

namespace CLX
{

	class Navmesh;

	class NavmeshCreator final
	{
		friend class Navmesh;

		static void CreateNavmesh(Navmesh& aNavmesh);

		static void CreateNodes(Navmesh& aNavmesh);
		static void CalculateConnections(Navmesh& aNavmesh);
		static void CreateWalls(Navmesh& aNavmesh);
		static void CalculatePathOffsets(NavmeshData& aNavmeshData, std::span<const NavmeshWall> aWalls);
		static void CalculateDimensions(Navmesh& aNavmesh);

		static void CreateNodeGrid(Navmesh& aNavmesh);
		static void CreateVertexGrid(Navmesh& aNavmesh);
		static void CreateWallGrid(Navmesh& aNavmesh);
	};
}