#include "Engine/Precompiled/EnginePch.hpp"
#include "Navmesh.hpp"
#include "NavmeshCreator.hpp"
#include "Engine/Math/UnitVector2.hpp"
#include "Engine/Math/Shapes/Triangle2.hpp"
#include "Engine/Math/ShapeMath.hpp"
#include "Engine/Utility/Intersection.hpp"

#include <assert.h>
#include <limits>

namespace CLX
{
	Navmesh::Navmesh()
	{
		NavmeshCreator::CreateNavmesh(*this);
	}

	Navmesh::Navmesh(const NavmeshData& navmeshData)
		: mNavmeshData(navmeshData)
	{
		if (mNavmeshData.mVertices.empty())
		{
			assert(false);
		}
		NavmeshCreator::CreateNavmesh(*this);
	}

	std::vector<Point3f> Navmesh::FindPath(const Point3f& startPosition, const NavmeshPositionData& endData) const
	{
		return Pathfinder::FindPath(*this, startPosition, endData);
	}

	std::vector<Point3f> Navmesh::FindPath(const Point3f& startPosition, const Point3f& endPosition) const
	{
		return Pathfinder::FindPath(*this, startPosition, endPosition);
	}

	bool Navmesh::IsInsideNavmesh(const Point3f& position) const
	{
		const Point2f pos2D(position.x, position.z);
		const std::optional<const GridCell<NavmeshNodeIndex>*> cell = mNodeGrid.GetCellByPosition(ToPoint2XZ(position));

		if (!cell) return false;

		for (const NavmeshNodeIndex nodeIndex : cell.value()->GetObjects())
		{
			if (IsInsideNode(mNodes[nodeIndex], pos2D))
			{
				return true;
			}
		}
		return false;
	}

	Point3f Navmesh::GetCorrectedPosition(const Point3f& position) const
	{
		std::optional<float> yPos = GetYPosition(position);
		return Point3f(position.x, yPos.value_or(position.y), position.z);
	}

	std::optional<float> Navmesh::GetYPosition(const Point3f& position) const
	{
		const std::optional<const GridCell<NavmeshNodeIndex>*> cell = mNodeGrid.GetCellByPosition(ToPoint2XZ(position));

		if (!cell)
		{
			return std::nullopt;
		}

		for (const NavmeshNodeIndex nodeIndex : cell.value()->GetObjects())
		{
			if (IsInsideNode(mNodes[nodeIndex], ToPoint2XZ(position)))
			{
				return GetPlaneYPos(mNodes[nodeIndex].mPlane, position.x, position.z);
			}
		}
		return std::nullopt;
	}

	bool Navmesh::IsStraightLine(const Point3f& startPosition, const Point3f& endPosition) const
	{
		PROFILER_FUNCTION(profiler::colors::Grey300);

		const LineSegment2f lineSegment = LineSegment2f::FromPoints(ToPoint2XZ(startPosition), ToPoint2XZ(endPosition));

		const AABB2f aabb2D = ToAABB2(lineSegment);
		const std::unordered_set<NavmeshWallIndex> wallsInside = mWallGrid.GetObjectsByAABB(aabb2D);

		for (const NavmeshWallIndex wallIndex : wallsInside)
		{
			const LineSegment2f wallSegment = GetLineSegment2DFromEdge(mWalls[wallIndex]);
			if (IntersectionLineSegment2DVsLineSegment2D(lineSegment, wallSegment))
			{
				return false;
			}
		}
		return true;
	}

	const NavmeshData& Navmesh::GetNavmeshData() const
	{
		return mNavmeshData;
	}

	NavmeshData& Navmesh::GetNavmeshData()
	{
		return mNavmeshData;
	}

	const std::vector<NavmeshWall>& Navmesh::GetWalls() const
	{
		return mWalls;
	}

	const AABB2f& Navmesh::GetDimensions() const
	{
		return mDimensions;
	}

	std::optional<Point3f> Navmesh::FindClosestPointInNavmesh(const Point3f& position, const Point3f& referencePoint) const
	{
		const Point2f pos2D = ToPoint2XZ(position);
		const Point2f referencePoint2D = ToPoint2XZ(referencePoint);
		const AABB2f aabb = CreateAABB2FromPoints(std::span<const Point2f>({ pos2D, referencePoint2D }));
		return FindClosestPointInNavmesh(position, aabb);
	}

	std::optional<Point3f> Navmesh::FindClosestPointInNavmesh(const Point3f& position, const Vector2f& searchExtent) const
	{
		const Point2f pos2D = ToPoint2XZ(position);
		const AABB2f aabb = AABB2f::FromCenterAndExtent(pos2D, searchExtent);
		return FindClosestPointInNavmesh(position, aabb);
	}

	std::optional<Point3f> Navmesh::FindClosestWallPoint(const Point3f& startPos, const Point3f& endPos) const
	{
		const Point2f startPos2D = ToPoint2XZ(startPos);
		const Point2f endPos2D = ToPoint2XZ(endPos);
		const AABB2f aabb = CreateAABB2FromPoints(std::span<const Point2f>({ startPos2D, endPos2D }));


		// Check walls
		std::unordered_set<NavmeshWallIndex> walls = mWallGrid.GetObjectsByAABB(aabb);

		struct ClosestPoint
		{
			float distance = 0.f;
			Point2f point;
		};

		LineSegment2f lineSegmentCheck = LineSegment2f::FromPoints(startPos2D, endPos2D);
		Point2f closestPoint;
		float closestDistance = std::numeric_limits<float>::max();
		for (const NavmeshWallIndex wallIndex : walls)
		{
			const LineSegment2f lineSegmentWall = GetLineSegment2DFromEdge(mWalls[wallIndex]);

			if (std::optional<Point2f> intersection = IntersectionLineSegment2DVsLineSegment2D(lineSegmentWall, lineSegmentCheck))
			{
				const float distance = Distance(startPos2D, intersection.value());
				if (distance < closestDistance)
				{
					closestDistance = distance;
					closestPoint = intersection.value() - GetDirection(lineSegmentCheck) * Wall_Offset;
				}
			}
		}

		if (closestDistance != std::numeric_limits<float>::max())
		{
			return GetCorrectedPosition(ToPoint3XZ(closestPoint));
		}
		return std::nullopt;
	}

	std::optional<Triangle3f> Navmesh::GetFaceByPosition(const Point3f& position) const
	{
		const NavmeshNodeIndex nodeIndex = GetNodeIndexFromPosition(position);
		if (nodeIndex == GetInvalidIndex<NavmeshNodeIndex>())
		{
			return std::nullopt;
		}
		return NodeToFace(nodeIndex);
	}

	std::optional<NavmeshPositionData> Navmesh::Raycast(const Ray3f& aRay) const
	{
		PROFILER_FUNCTION(profiler::colors::GreenA400);

		Point3f endPos;
		NavmeshNodeIndex endNodeIndex = GetInvalidIndex<NavmeshNodeIndex>();
		float closestHitDistance = std::numeric_limits<float>::max();
		for (NavmeshNodeIndex nodeIndex = 0; nodeIndex < mNodes.size(); ++nodeIndex)
		{
			const NavmeshNode& node = mNodes[nodeIndex];
			if (std::optional<RayHitResult> intersection = IntersectionRayPlane(aRay, node.mPlane))
			{
				const float hitDistance = intersection->distance;

				if (hitDistance < closestHitDistance && IsInsideNode(node, ToPoint2XZ(intersection->hitPoint)))
				{
					endNodeIndex = nodeIndex;
					endPos = intersection->hitPoint;

					closestHitDistance = hitDistance;
				}
			}
		}

		if (endNodeIndex == GetInvalidIndex<NavmeshNodeIndex>())
		{
			return std::nullopt;
		}

		return NavmeshPositionData{ endNodeIndex, endPos, ToPoint2XZ(endPos) };
	}

	std::optional<Point3f> Navmesh::FindClosestPointInNavmesh(const Point3f& position, const AABB2f& aSearchField) const
	{
		const Point2f pos2D = ToPoint2XZ(position);

		// Check walls
		const std::unordered_set<NavmeshWallIndex> walls = mWallGrid.GetObjectsByAABB(aSearchField);

		Point2f closestPoint;
		float closestDistance = FLT_MAX;
		for (const NavmeshWallIndex wallIndex : walls)
		{
			const LineSegment2f lineSegmentWall = GetLineSegment2DFromEdge(mWalls[wallIndex]);

			const UnitVector2f normal = GetNormal(lineSegmentWall);
			LineSegment2f lineSegmentCheck = LineSegment2f::FromPoints(pos2D + (-normal) * 1000.f, pos2D + normal * 1000.f);
			if (std::optional<Point2f> intersection = IntersectionLineSegment2DVsLineSegment2D(lineSegmentWall, lineSegmentCheck))
			{
				const float distance = Distance(pos2D, intersection.value());
				if (distance < closestDistance)
				{
					closestPoint = intersection.value() - GetDirection(lineSegmentCheck) * Wall_Offset;
					closestDistance = distance;
				}
			}
		}

		// Check vertices
		const std::unordered_set<NavmeshVertexIndex> vertexIndices = mVertexGrid.GetObjectsByAABB(aSearchField);

		UnitVector3f offset;
		for (const NavmeshVertexIndex vertexIndex : vertexIndices)
		{
			const float distance = Distance(pos2D, mNavmeshData.mVertices[vertexIndex]);
			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestPoint = mNavmeshData.mVertices[vertexIndex];
				offset = mNavmeshData.mPathOffsetDirections[vertexIndex];
			}
		}

		if (closestDistance != FLT_MAX)
		{
			const Point3f point3D = ToPoint3XZ(closestPoint);
			const float yPos = GetYPosition(point3D).value();
			return ToPoint3XZ(closestPoint, yPos)/* - offset * Path_Offset*/;
		}


		return std::nullopt;
	}

	NavmeshPositionData Navmesh::GetPositionData(const Point3f& position) const
	{
		const NavmeshNodeIndex nodeIndex = GetNodeIndexFromPosition(position);
		NavmeshPositionData data;
		data.mNodeIndex = nodeIndex;
		if (nodeIndex != GetInvalidIndex<NavmeshNodeIndex>())
		{
			const Point3f pos = GetCorrectedPosition(position);
			data.m2DPosition = ToPoint2XZ(position);
			data.m3DPosition = pos;
		}
		return data;
	}

	Triangle3f Navmesh::NodeToFace(const NavmeshNodeIndex nodeIndex) const
	{
		const NavmeshNode& node = mNodes[nodeIndex];
		const Point3f& vertex1 = mNavmeshData.m3DVertices[node.mVertexIndices[0]];
		const Point3f& vertex2 = mNavmeshData.m3DVertices[node.mVertexIndices[1]];
		const Point3f& vertex3 = mNavmeshData.m3DVertices[node.mVertexIndices[2]];
		return Triangle3f(vertex1, vertex2, vertex3);
	}

	NavmeshNodeIndex Navmesh::GetNodeIndexFromPosition(const Point3f& position) const
	{
		const Point2f pos2D = ToPoint2XZ(position);
		std::vector<NavmeshNodeIndex> insideNodes;

		const std::optional<const GridCell<NavmeshNodeIndex>*> cell = mNodeGrid.GetCellByPosition(ToPoint2XZ(position));

		if (!cell)
		{
			return GetInvalidIndex<NavmeshNodeIndex>();
		}

		for (const NavmeshNodeIndex nodeIndex : cell.value()->GetObjects())
		{
			if (IsInsideNode(mNodes[nodeIndex], pos2D))
			{
				insideNodes.push_back(nodeIndex);
			}
		}
		if (insideNodes.empty())
		{
			return GetInvalidIndex<NavmeshNodeIndex>();
		}
		float closestDistance = Distance(mNodes[insideNodes[0]].mCenter3D, position);

		NavmeshNodeIndex closestNodeIndex = insideNodes[0];
		for (size_t i = 1; i < insideNodes.size(); ++i)
		{
			const float distance = Distance(mNodes[insideNodes[i]].mCenter3D, position);
			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestNodeIndex = insideNodes[i];
			}
		}
		return closestNodeIndex;
	}

	bool Navmesh::IsInsideNode(const NavmeshNode& node, const Point2f& position) const
	{
		int insideCounter = 0;
		for (NavmeshVertexIndex vertexIndex = 0; vertexIndex < Vertices_In_Triangle; vertexIndex++)
		{
			const int otherIndex = (vertexIndex + 1) % Vertices_In_Triangle;

			const Point2f& vertex1Pos = mNavmeshData.mVertices[node.mVertexIndices[vertexIndex]];
			const Point2f& vertex2Pos = mNavmeshData.mVertices[node.mVertexIndices[otherIndex]];
			const UnitVector2f direction = UnitVector2f(vertex2Pos - vertex1Pos);
			const UnitVector2f normal = { -direction.Y(), direction.X() };
			const bool isInsideLine = Dot(position - vertex2Pos, normal) <= 0;

			if (isInsideLine)
			{
				insideCounter++;
			}
		}
		return insideCounter == Vertices_In_Triangle;
	}

	bool Navmesh::IsVertexConnectedToNode(const NavmeshVertexIndex vertexIndex, const NavmeshNodeIndex nodeIndex) const
	{
		if (vertexIndex < 0 || vertexIndex >= mNavmeshData.mVertices.size())
		{
			return false;
		}
		for (int i = 0; i < Vertices_In_Triangle; i++)
		{
			if (mNodes[nodeIndex].mVertexIndices[i] == vertexIndex)
			{
				return true;
			}
		}
		return false;
	}

	bool Navmesh::AreNodesConnected(const NavmeshNodeIndex node1, const NavmeshNodeIndex node2) const
	{
		if (node1 == node2)
		{
			return false;
		}

		for (NavmeshVertexIndex i = 0; i < Vertices_In_Triangle; i++)
		{
			if (mNodes[node1].mConnections[i] == node2)
			{
				return true;
			}
		}
		return false;
	}

	bool Navmesh::AreAllVerticesConnectedToNode(const NavmeshNodeIndex nodeIndex, const NavmeshVertexIndex vertexIndex1, const NavmeshVertexIndex vertexIndex2, const NavmeshVertexIndex vertexIndex3) const
	{
		bool index1Exists = false;
		bool index2Exists = false;
		bool index3Exists = false;

		const NavmeshNode& node = mNodes[nodeIndex];
		for (const NavmeshVertexIndex vertexIndex : node.mVertexIndices)
		{
			index1Exists |= (vertexIndex == vertexIndex1);
			index2Exists |= (vertexIndex == vertexIndex2);
			index3Exists |= (vertexIndex == vertexIndex3);
		}
		return index1Exists && index2Exists && index3Exists;
	}

	std::optional<NavmeshEdge> Navmesh::GetEdgeBetweenNodes(const NavmeshNodeIndex nodeIndex1, const NavmeshNodeIndex nodeIndex2) const
	{
		if (nodeIndex1 == nodeIndex2)
		{
			return std::nullopt;
		}
		NavmeshEdge edge;
		edge.first = GetInvalidIndex<NavmeshVertexIndex>();
		edge.second = GetInvalidIndex<NavmeshVertexIndex>();
		
		for (const NavmeshVertexIndex vertexIndex1 : mNodes[nodeIndex1].mVertexIndices)
		{
			for (const NavmeshVertexIndex vertexIndex2 : mNodes[nodeIndex2].mVertexIndices)
			{
				if (vertexIndex1 != vertexIndex2)
				{
					continue;
				}

				if (edge.first == GetInvalidIndex<NavmeshNodeIndex>())
				{
					edge.first = vertexIndex1;
					break;
				}
				else
				{
					edge.second = vertexIndex1;
					return edge;
				}
			}
		}
		return std::nullopt;
	}

	std::optional<LineSegment2f> Navmesh::GetEdgeLineSegment2DFromNodes(const NavmeshNodeIndex nodeIndex1, const NavmeshNodeIndex nodeIndex2) const
	{
		std::optional<NavmeshEdge> edge = GetEdgeBetweenNodes(nodeIndex1, nodeIndex2);
		if (edge)
		{
			return GetLineSegment2DFromEdge(edge.value());
		}
		return std::nullopt;
	}

	LineSegment2f Navmesh::GetLineSegment2DFromEdge(const NavmeshEdge& edge) const
	{
		return LineSegment2f::FromPoints(mNavmeshData.mVertices[edge.first], mNavmeshData.mVertices[edge.second]);
	}
}