#pragma once
#include "NavmeshData.hpp"
#include <vector>

namespace CLX
{

	class Navmesh;

	class Funneling
	{
		enum class eMover
		{
			Left,
			Right,
			None
		};

		struct FunnelPoint
		{
			NavmeshVertexIndex vertexIndex = GetInvalidIndex<NavmeshNodeIndex>();
			std::size_t portalIndex = GetInvalidIndex<std::size_t>();
		};

		struct Portal
		{
			eMover mover = eMover::None;
			FunnelPoint left;
			FunnelPoint right;
		};

		struct Apex
		{
			NavmeshVertexIndex myIndex;
			Vector2f myPosition;
		};


	public:

		Funneling() = delete;
		static std::vector<NavmeshVertexIndex> FindPath(const Navmesh& aNavmesh, const NavmeshPositionData& aStartData, const NavmeshPositionData& anEndData, const std::vector<NavmeshNodeIndex>& anAStarPath);

	};
}