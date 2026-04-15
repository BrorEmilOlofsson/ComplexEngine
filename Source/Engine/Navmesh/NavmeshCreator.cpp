#include "Engine/Precompiled/EnginePch.hpp"
#include "NavmeshCreator.hpp"
#include "NavmeshData.hpp"
#include "Navmesh.hpp"

namespace CLX
{
	void NavmeshCreator::CreateNavmesh(Navmesh& navmesh)
	{
		PROFILER_FUNCTION(profiler::colors::Grey300);
		CreateNodes(navmesh);
		CalculateConnections(navmesh);
		CreateWalls(navmesh);
		CalculatePathOffsets(navmesh.mNavmeshData, navmesh.mWalls);
		CalculateDimensions(navmesh);
		CreateNodeGrid(navmesh);
		CreateVertexGrid(navmesh);
		CreateWallGrid(navmesh);
	}


	void NavmeshCreator::CreateNodes(Navmesh& navmesh)
	{
		PROFILER_FUNCTION(profiler::colors::Grey300);
		for (NavmeshNodeIndex nodeIndex = 0; nodeIndex < navmesh.mNavmeshData.mIndices.size(); nodeIndex += Vertices_In_Triangle)
		{
			NavmeshNode& newNode = navmesh.mNodes.emplace_back();

			newNode.mVertexIndices[0] = navmesh.mNavmeshData.mIndices[nodeIndex + 0ull];
			newNode.mVertexIndices[1] = navmesh.mNavmeshData.mIndices[nodeIndex + 1ull];
			newNode.mVertexIndices[2] = navmesh.mNavmeshData.mIndices[nodeIndex + 2ull];

			// Calculate center
			const Point2f& pos1 = navmesh.mNavmeshData.mVertices[newNode.mVertexIndices[0]];
			const Point2f& pos2 = navmesh.mNavmeshData.mVertices[newNode.mVertexIndices[1]];
			const Point2f& pos3 = navmesh.mNavmeshData.mVertices[newNode.mVertexIndices[2]];
			newNode.mCenter = AveragePoint(pos1, pos2, pos3);

			// Calculate center
			const Point3f& pos13D = navmesh.mNavmeshData.m3DVertices[newNode.mVertexIndices[0]];
			const Point3f& pos23D = navmesh.mNavmeshData.m3DVertices[newNode.mVertexIndices[1]];
			const Point3f& pos33D = navmesh.mNavmeshData.m3DVertices[newNode.mVertexIndices[2]];
			newNode.mCenter3D = AveragePoint(pos13D, pos23D, pos33D);

			newNode.mPlane = Planef::FromPoints(newNode.mCenter3D, pos23D, pos33D);

			if (newNode.mPlane.GetNormal().Y() <= 0)
			{
				assert(false && "Normal of navmesh face points down");
			}

		}
	}

	constexpr std::size_t GetSharedVertexCount(const NavmeshNode& node1, const NavmeshNode& node2) noexcept
	{
		return std::ranges::count_if(node1.mVertexIndices, [&](auto idx1)
			{
				return std::ranges::any_of(node2.mVertexIndices, [&](auto idx2)
					{
						return idx1 == idx2;
					});
			});
	}

	void NavmeshCreator::CalculateConnections(Navmesh& navmesh)
	{
		PROFILER_FUNCTION(profiler::colors::Grey300);
		const size_t size = navmesh.mNodes.size();
		for (NavmeshNodeIndex currentNodeIndex = 0; currentNodeIndex < size; ++currentNodeIndex)
		{
			NavmeshNode& node = navmesh.mNodes[currentNodeIndex];
			NavmeshNodeIndex startNode = 0;
			for (NavmeshNodeIndex& nodeConnectionIndex : node.mConnections)
			{
				NavmeshNodeIndex connectionIndex = GetInvalidIndex<NavmeshNodeIndex>();

				for (NavmeshNodeIndex i = startNode; i < size; ++i)
				{
					if (currentNodeIndex == i) continue;

					const NavmeshNode& connection = navmesh.mNodes[i];

					if (GetSharedVertexCount(node, connection) == 2)
					{
						connectionIndex = i;
						startNode = i + 1;
						break;
					}
				}
				nodeConnectionIndex = connectionIndex;
			}
		}
	}

	void NavmeshCreator::CreateWalls(Navmesh& navmesh)
	{
		PROFILER_FUNCTION(profiler::colors::Grey300);
		for (size_t i = 0; i < navmesh.mNodes.size(); ++i)
		{
			for (NavmeshVertexIndex j = 0; j < Vertices_In_Triangle; ++j)
			{
				const NavmeshVertexIndex otherIndex = (j + 1) % Vertices_In_Triangle;

				const NavmeshVertexIndex a = navmesh.mNodes[i].mVertexIndices[j];
				const NavmeshVertexIndex b = navmesh.mNodes[i].mVertexIndices[otherIndex];


				bool isWall = true;

				for (size_t k = 0; k < Vertices_In_Triangle && isWall; ++k)
				{
					const NavmeshNodeIndex connectionNodeIndex = navmesh.mNodes[i].mConnections[k];

					for (NavmeshVertexIndex l = 0; l < Vertices_In_Triangle; ++l)
					{
						const NavmeshVertexIndex otherIndex2 = (l + 1) % Vertices_In_Triangle;

						if (connectionNodeIndex == GetInvalidIndex<NavmeshVertexIndex>())
						{
							continue;
						}

						const NavmeshVertexIndex c = navmesh.mNodes[connectionNodeIndex].mVertexIndices[l];
						const NavmeshVertexIndex d = navmesh.mNodes[connectionNodeIndex].mVertexIndices[otherIndex2];

						if (a == d && b == c)
						{
							isWall = false;
							break;
						}
					}
				}

				if (isWall)
				{
					NavmeshWall wall(a, b);

					navmesh.mWalls.push_back(wall);
				}
			}
		}

		for (size_t i = 0; i < navmesh.mWalls.size(); ++i)
		{
			bool connectionExists = false;
			for (size_t j = 0; j < navmesh.mWalls.size(); ++j)
			{
				if (navmesh.mWalls[i].first == navmesh.mWalls[j].second)
				{
					connectionExists = true;
					break;
				}
			}
			if (!connectionExists)
			{
				assert(false && "There are walls inside the navmesh!");
			}
		}
	}

	void NavmeshCreator::CalculatePathOffsets(NavmeshData& navmeshData, std::span<const NavmeshWall> walls)
	{
		PROFILER_FUNCTION(profiler::colors::Grey300);
		navmeshData.mPathOffsetDirections.resize(navmeshData.m3DVertices.size());

		for (const NavmeshWall& wall1 : walls)
		{
			const size_t currentVertexIndex = wall1.first;
			const Point2f& leftPos = navmeshData.mVertices[wall1.second];
			const Point2f& middlePos = navmeshData.mVertices[currentVertexIndex];
			std::optional<Point2f> rightPos = std::nullopt;
			for (const NavmeshWall& wall2 : walls)
			{
				if (wall1.first == wall2.second)
				{
					rightPos = navmeshData.mVertices[wall2.first];
				}
			}
			if (!rightPos)
			{
				assert(false && "Navmesh is broken, probably!");
				return;
			}

			const UnitVector2f middleToLeft = GetUnitVector(middlePos, leftPos);
			const UnitVector2f middleToRight = GetUnitVector(middlePos, *rightPos);

			const Vector2f added = middleToLeft + middleToRight;
			Vector2f dir;
			if (LengthSquared(added) == 0.f)
			{
				dir = GetPerpendicularVector(middleToLeft) * 1.f;
			}
			else
			{
				dir = ToNormalized(-added);
			}

			const Vector3f dir3D = ToVector3XZ(dir);

			navmeshData.mPathOffsetDirections[currentVertexIndex] = UnitVector3f(dir3D);
		}
	}

	void NavmeshCreator::CalculateDimensions(Navmesh& navmesh)
	{
		if (!navmesh.mNavmeshData.mVertices.empty())
		{
			navmesh.mDimensions = CreateAABB2FromPoints(std::span<const Point2f>(navmesh.mNavmeshData.mVertices));
		}

		navmesh.mDimensions.SetMin(navmesh.mDimensions.GetMin() + Vector2f(-Navmesh::Dimensions_Offset, -Navmesh::Dimensions_Offset));
		navmesh.mDimensions.SetMax(navmesh.mDimensions.GetMax() + Vector2f(Navmesh::Dimensions_Offset, Navmesh::Dimensions_Offset));
	}

	void NavmeshCreator::CreateNodeGrid(Navmesh& navmesh)
	{
		PROFILER_FUNCTION(profiler::colors::Grey300);
		const size_t nodeSize = navmesh.mNodes.size();
		const Vector2f extent = navmesh.mDimensions.GetExtent();
		const unsigned int numTilesPerDimension = static_cast<unsigned int>(Sqrt(static_cast<float>(nodeSize))) + 1;
		const Vector2u gridSize(numTilesPerDimension, numTilesPerDimension);
		const Vector2f cellSize(extent.x / gridSize.x, extent.y / gridSize.y);
		const Point2f startPos = navmesh.mDimensions.GetMin();
		navmesh.mNodeGrid = Grid2<NavmeshNodeIndex>(gridSize, cellSize, startPos);

		for (NavmeshNodeIndex nodeIndex = 0; nodeIndex < navmesh.mNodes.size(); ++nodeIndex)
		{
			std::vector<Point2f> points;
			for (const NavmeshVertexIndex vertexIndex : navmesh.mNodes[nodeIndex].mVertexIndices)
			{
				const Point2f& vertexPos = navmesh.mNavmeshData.mVertices[vertexIndex];
				points.push_back(vertexPos);
			}

			const AABB2f nodeDimensions = CreateAABB2FromPoints(std::span<const Point2f>(points));

			navmesh.mNodeGrid.AddObjectToCellsInsideAABB(nodeIndex, nodeDimensions);
		}
	}

	void NavmeshCreator::CreateVertexGrid(Navmesh& navmesh)
	{
		PROFILER_FUNCTION(profiler::colors::Grey300);
		const size_t vertexSize = navmesh.mNavmeshData.mVertices.size();
		const Vector2f diffMaxMin = navmesh.mDimensions.GetMax() - navmesh.mDimensions.GetMin();
		const unsigned int numTilesPerDimension = static_cast<unsigned int>(sqrt(vertexSize)) + 1;
		const Vector2u gridSize(numTilesPerDimension, numTilesPerDimension);
		const Vector2f cellSize(diffMaxMin.x / gridSize.x, diffMaxMin.y / gridSize.y);
		const Point2f startPos = navmesh.mDimensions.GetMin();
		navmesh.mVertexGrid = Grid2<NavmeshVertexIndex>(gridSize, cellSize, startPos);

		for (NavmeshVertexIndex i = 0; i < navmesh.mNavmeshData.mVertices.size(); ++i)
		{
			navmesh.mVertexGrid.GetCellByPosition(navmesh.mNavmeshData.mVertices[i]).value()->Add(i);
		}
	}

	void NavmeshCreator::CreateWallGrid(Navmesh& navmesh)
	{
		PROFILER_FUNCTION(profiler::colors::Grey300);
		AABB2f dimensions = ScaleAABB(navmesh.mDimensions, 1.5f);

		const size_t wallSize = navmesh.mWalls.size();
		const Vector2f diffMaxMin = dimensions.GetExtent();
		const unsigned int numTilesPerDimension = static_cast<unsigned int>(sqrt(wallSize)) + 10u;
		const Vector2u gridSize(numTilesPerDimension, numTilesPerDimension);
		const Vector2f cellSize(diffMaxMin.x / gridSize.x, diffMaxMin.y / gridSize.y);
		const Point2f startPos = dimensions.GetMin();
		navmesh.mWallGrid = Grid2<NavmeshWallIndex>(gridSize, cellSize, startPos);

		for (NavmeshWallIndex wallIndex = 0; wallIndex < navmesh.mWalls.size(); ++wallIndex)
		{
			const NavmeshWall& wall = navmesh.mWalls[wallIndex];
			std::vector<Point2f> points =
			{
				navmesh.mNavmeshData.mVertices[wall.first],
				navmesh.mNavmeshData.mVertices[wall.second]
			};

			const AABB2f wallDimensions = CreateAABB2FromPoints(std::span<const Point2f>(points));

			navmesh.mWallGrid.AddObjectToCellsInsideAABB(wallIndex, wallDimensions);
		}

#ifdef _DEBUG
		//navmesh->myDebugWallGrid.Init<NavmeshWallIndex>(*navmesh->myWallGrid, 0.f, eLineColor::eWheat);
#endif
	}
}
