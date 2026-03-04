#include "Engine/Precompiled/EnginePch.hpp"
#include "Funneling.hpp"
#include "Navmesh.hpp"

namespace CLX
{

	static void CalculateLeftAndRightIndex(const Navmesh& navmesh, NavmeshVertexIndex& leftPoint, NavmeshVertexIndex& rightPoint, const NavmeshNodeIndex firstIndex, const NavmeshNodeIndex secondIndex)
	{
		const NavmeshNode& node1 = navmesh.GetNodes()[firstIndex];
		const NavmeshNode& node2 = navmesh.GetNodes()[secondIndex];

		unsigned int firstMatchedIndex = 0;
		unsigned int secondMatchedIndex = 0;

		for (unsigned int i = 0; i < Vertices_In_Triangle; ++i)
		{
			unsigned int& currentMatchedIndex = firstMatchedIndex == 0 ? firstMatchedIndex : secondMatchedIndex;

			for (unsigned int j = 0; j < Vertices_In_Triangle; ++j)
			{
				if (node1.mVertexIndices[i] == node2.mVertexIndices[j])
				{
					currentMatchedIndex = i;
					break;
				}
			}
		}

		if (firstMatchedIndex < 3 && secondMatchedIndex < 3)
		{
			if (((firstMatchedIndex + 1) % 3) == secondMatchedIndex) //Clockwise
			{
				leftPoint = node1.mVertexIndices[firstMatchedIndex];
				rightPoint = node1.mVertexIndices[secondMatchedIndex];
			}
			else
			{
				leftPoint = node1.mVertexIndices[secondMatchedIndex];
				rightPoint = node1.mVertexIndices[firstMatchedIndex];
			}
		}
	}


	std::vector<NavmeshVertexIndex> Funneling::FindPath(const Navmesh& navmesh, const NavmeshPositionData& startData, const NavmeshPositionData& endData, const std::vector<NavmeshNodeIndex>& aStarPath)
	{
		PROFILER_FUNCTION(profiler::colors::Yellow300);

		const std::vector<Point2f>& vertices = navmesh.GetNavmeshData().mVertices;

		// Create portals
		std::vector<Portal> portals(aStarPath.size() - 1);

		for (size_t i = 0; i < portals.size(); ++i)
		{
			Portal& portal = portals[i];
			CalculateLeftAndRightIndex(navmesh, portal.left.vertexIndex, portal.right.vertexIndex, aStarPath[i], aStarPath[i + 1]);
			portal.left.portalIndex = i;
			portal.right.portalIndex = i;
		}

		for (size_t i = 1; i < portals.size(); ++i)
		{
			if (portals[i].left.vertexIndex == portals[i - 1].left.vertexIndex)
			{
				portals[i].mover = eMover::Right;
			}
			else
			{
				portals[i].mover = eMover::Left;
			}
		}

		Point2f apex = startData.m2DPosition;

		Radiansf currentAngle = CalculateAngle(navmesh.GetNavmeshData().mVertices[portals[0].left.vertexIndex] - apex, navmesh.GetNavmeshData().mVertices[portals[0].right.vertexIndex] - apex);


		std::vector<NavmeshVertexIndex> wayPoints;


		std::vector<FunnelPoint> leftSaved;
		std::vector<FunnelPoint> rightSaved;

		for (size_t i = 1; i <= portals.size(); ++i)
		{
			if (i == portals.size())
			{
				const Vector2f endVector = endData.m2DPosition - apex;

				if (!rightSaved.empty())
				{
					const Radiansf angle = CalculateAngle(endVector, vertices[rightSaved.front().vertexIndex] - apex);

					if (angle < Radiansf(0))
					{
						apex = vertices[rightSaved.front().vertexIndex];
						i = rightSaved.front().portalIndex + 1ull;

						wayPoints.push_back(rightSaved.front().vertexIndex);

						currentAngle = CalculateAngle(vertices[portals[i].left.vertexIndex] - apex, vertices[portals[i].right.vertexIndex] - apex);

						rightSaved.clear();
						leftSaved.clear();

						continue;
					}
				}
				else
				{
					const Radiansf angle = CalculateAngle(endVector, vertices[portals.back().right.vertexIndex] - apex);

					if (angle < Radiansf(0))
					{
						apex = vertices[portals.back().right.vertexIndex];
						i = portals.back().right.portalIndex;

						wayPoints.push_back(portals.back().right.vertexIndex);

						currentAngle = CalculateAngle(vertices[portals[i].left.vertexIndex] - apex, vertices[portals[i].right.vertexIndex] - apex);

						rightSaved.clear();
						leftSaved.clear();

						continue;
					}
				}

				if (!leftSaved.empty())
				{
					const Radiansf angle = CalculateAngle(vertices[leftSaved.front().vertexIndex] - apex, endVector);

					if (angle < Radiansf(0))
					{
						apex = vertices[leftSaved.front().vertexIndex];
						i = leftSaved.front().portalIndex + 1ull;

						wayPoints.push_back(leftSaved.front().vertexIndex);

						currentAngle = CalculateAngle(vertices[portals[i].left.vertexIndex] - apex, vertices[portals[i].right.vertexIndex] - apex);


						rightSaved.clear();
						leftSaved.clear();
					}
				}
				else
				{
					const Radiansf angle = CalculateAngle(vertices[portals.back().left.vertexIndex] - apex, endVector);

					if (angle < Radiansf(0))
					{
						apex = vertices[portals.back().left.vertexIndex];
						i = portals.back().left.portalIndex;

						wayPoints.push_back(portals.back().left.vertexIndex);

						currentAngle = CalculateAngle(vertices[portals[i].left.vertexIndex] - apex, vertices[portals[i].right.vertexIndex] - apex);

						rightSaved.clear();
						leftSaved.clear();
					}
				}

				if (i == portals.size())
					break;

				continue;
			}

			FunnelPoint leftt = portals[i].left;
			FunnelPoint rightt = portals[i].right;

			if (leftSaved.size() > 0)
				leftt = leftSaved.front();
			if (rightSaved.size() > 0)
				rightt = rightSaved.front();

			switch (portals[i].mover)
			{
			case eMover::Right:
				rightt = portals[i].right;
				break;
			case eMover::Left:
				leftt = portals[i].left;
				break;
			}

			const Radiansf nextAngle = CalculateAngle(vertices[leftt.vertexIndex] - apex, vertices[rightt.vertexIndex] - apex);

			if (nextAngle < Radiansf(0))
			{
				switch (portals[i].mover)
				{
				case eMover::Right:
				{
					apex = vertices[leftt.vertexIndex];
					size_t nextPortal = leftt.portalIndex;
					for (size_t j = leftt.portalIndex + 1ull; j < portals.size(); ++j)
					{
						if (portals[j].left.vertexIndex != leftt.vertexIndex)
						{
							nextPortal = j;
							break;
						}
					}
					i = nextPortal;
					wayPoints.push_back(leftt.vertexIndex);

					break;
				}
				case eMover::Left:
				{
					apex = vertices[rightt.vertexIndex];
					size_t nextPortal = leftt.portalIndex;
					for (size_t j = rightt.portalIndex + 1ull; j < portals.size(); ++j)
					{
						if (portals[j].right.vertexIndex != rightt.vertexIndex)
						{
							nextPortal = j;
							break;
						}
					}
					i = nextPortal;
					wayPoints.push_back(rightt.vertexIndex);
					break;
				}
				}


				currentAngle = CalculateAngle(vertices[portals[i].left.vertexIndex] - apex, vertices[portals[i].right.vertexIndex] - apex);


				rightSaved.clear();
				leftSaved.clear();
			}
			else if (nextAngle > currentAngle)
			{
				switch (portals[i].mover)
				{
				case eMover::Right:
					rightSaved.push_back(portals[i - 1].right);
					break;
				case eMover::Left:
					leftSaved.push_back(portals[i - 1].left);
					break;
				}
			}
			else if (nextAngle < currentAngle && nextAngle >= Radiansf(0))
			{
				switch (portals[i].mover)
				{
				case eMover::Right:
					rightSaved.clear();
					break;
				case eMover::Left:
					leftSaved.clear();
					break;
				}

				currentAngle = nextAngle;
			}
		}


		return wayPoints;
	}
}

