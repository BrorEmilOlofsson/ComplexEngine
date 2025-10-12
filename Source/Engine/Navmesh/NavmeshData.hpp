#pragma once
#include <vector>
#include "Utility/Math/Point2.hpp"
#include "Utility/Math/Point3.hpp"
#include "NavmeshDefines.hpp"
#include "Utility/Shapes/Plane.hpp"

namespace Simple
{

	constexpr unsigned int Vertices_In_Triangle = 3;

	using NavmeshNodeIndex = unsigned int;
	using NavmeshVertexIndex = unsigned int;

	using NavmeshEdge = std::pair<NavmeshVertexIndex, NavmeshVertexIndex>;
	using NavmeshWall = NavmeshEdge;

	enum class eNavmeshNodeState
	{
		Walkable,
		Unwalkable
	};

	struct NavmeshNode
	{
		Point2f mCenter;
		Point3f mCenter3D;
		Planef mPlane;
		std::array<NavmeshNodeIndex, Vertices_In_Triangle> mConnections{};
		std::array<NavmeshVertexIndex, Vertices_In_Triangle> mVertexIndices{};
		eNavmeshNodeState mState = eNavmeshNodeState::Walkable;
	};


	struct NavmeshData
	{
		std::vector<Point2f> mVertices;
		std::vector<Point3f> m3DVertices;
		std::vector<UnitVector3f> mPathOffsetDirections;
		std::vector<NavmeshVertexIndex> mIndices;
	};

	struct NavmeshPositionData
	{
		NavmeshNodeIndex mNodeIndex;
		Point3f m3DPosition;
		Point2f m2DPosition;
	};
}
