#pragma once
#include "NavmeshData.hpp"
#include "Pathfinder.hpp"

#include "Utility/Shapes/LineSegment3.hpp"
#include "Utility/Shapes/AABB2.hpp"

#include <vector>
#include <queue>
#include <memory>
#include <optional>
#include <filesystem>

#include "Utility/Shapes/LineSegment2.hpp"
#include "Utility/Shapes/Triangle3.hpp"
#include "Utility/Grid/Grid2.hpp"
#include "Utility/Shapes/Ray3.hpp"

namespace Simple
{
	using NavmeshWallIndex = int;

	class Navmesh final
	{
		friend class NavmeshAStar;
		friend class Pathfinder;
		friend class NavmeshCreator;
	public:

		Navmesh();
		Navmesh(const NavmeshData& navmeshData);

		[[nodiscard]] std::vector<Point3f> FindPath(const Point3f& startPosition, const NavmeshPositionData& endData) const;
		[[nodiscard]] std::vector<Point3f> FindPath(const Point3f& startPosition, const Point3f& endPosition) const;
		
		[[nodiscard]] bool IsInsideNavmesh(const Point3f& position) const;
		[[nodiscard]] Point3f GetCorrectedPosition(const Point3f& position) const;
		[[nodiscard]] std::optional<float> GetYPosition(const Point3f& position) const;
		[[nodiscard]] bool IsStraightLine(const Point3f& startPosition, const Point3f& endPosition) const;

		[[nodiscard]] constexpr const std::vector<NavmeshNode>& GetNodes() const;
		[[nodiscard]] const NavmeshData& GetNavmeshData() const;
		[[nodiscard]] NavmeshData& GetNavmeshData();
		[[nodiscard]] const std::vector<NavmeshWall>& GetWalls() const;

		[[nodiscard]] const AABB2f& GetDimensions() const;

		[[nodiscard]] std::optional<Point3f> FindClosestPointInNavmesh(const Point3f& position, const Point3f& referencePoint) const;
		[[nodiscard]] std::optional<Point3f> FindClosestPointInNavmesh(const Point3f& position, const Vector2f& searchExtents) const;
		[[nodiscard]] std::optional<Point3f> FindClosestWallPoint(const Point3f& startPos, const Point3f& endPos) const;

		[[nodiscard]] std::optional<Triangle3f> GetFaceByPosition(const Point3f& position) const;
		[[nodiscard]] std::optional<NavmeshPositionData> Raycast(const Ray3f& ray) const;

		[[nodiscard]] const Grid2<NavmeshNodeIndex>& GetNodeGrid() const { return mNodeGrid; }
		[[nodiscard]] const Grid2<NavmeshVertexIndex>& GetVertexGrid() const { return mVertexGrid; }
		[[nodiscard]] const Grid2<NavmeshWallIndex>& GetWallGrid() const { return mWallGrid; }

	private:

		std::optional<Point3f> FindClosestPointInNavmesh(const Point3f& position, const AABB2f& searchField) const;
		
		NavmeshPositionData GetPositionData(const Point3f& position) const;

		Triangle3f NodeToFace(const NavmeshNodeIndex nodeIndex) const;

		NavmeshNodeIndex GetNodeIndexFromPosition(const Point3f& position) const;
		bool IsInsideNode(const NavmeshNode& node, const Point2f& position) const;
		bool IsVertexConnectedToNode(const NavmeshVertexIndex aVertexIndex, const NavmeshNodeIndex nodeIndex) const;
		bool AreNodesConnected(const NavmeshNodeIndex node1, const NavmeshNodeIndex node2) const;
		bool AreAllVerticesConnectedToNode(const NavmeshNodeIndex nodeIndex, const NavmeshVertexIndex vertexIndex1, const NavmeshVertexIndex vertexIndex2, const NavmeshVertexIndex aVertexIndex3) const;
		std::optional<NavmeshEdge> GetEdgeBetweenNodes(const NavmeshNodeIndex nodeIndex1, const NavmeshNodeIndex nodeIndex2) const;
		std::optional<LineSegment2f> GetEdgeLineSegment2DFromNodes(const NavmeshNodeIndex nodeIndex1, const NavmeshNodeIndex nodeIndex2) const;
		LineSegment2f GetLineSegment2DFromEdge(const NavmeshEdge& edge) const;

	private:

		std::vector<NavmeshNode> mNodes;
		std::vector<NavmeshWall> mWalls;
		NavmeshData mNavmeshData;

		Grid2<NavmeshNodeIndex> mNodeGrid;
		Grid2<NavmeshVertexIndex> mVertexGrid;
		Grid2<NavmeshWallIndex> mWallGrid;

		AABB2f mDimensions;

		static constexpr float Wall_Offset = 0.0f;
		static constexpr float Closest_Point_AABB_Size = 10.f;
		static constexpr float Dimensions_Offset = 0.5f;
		static constexpr float Path_Offset = 0.0f;
	};

	constexpr const std::vector<NavmeshNode>& Navmesh::GetNodes() const
	{
		return mNodes;
	}
}
