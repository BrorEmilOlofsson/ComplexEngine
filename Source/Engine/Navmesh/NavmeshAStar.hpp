#pragma once
#include "NavmeshData.hpp"

#include <queue>
#include <unordered_set>
#include <set>
#include <span>
#include <vector>

namespace Simple
{

	class Navmesh;

	class NavmeshAStar
	{
	public:
		enum class eNodeStatus
		{
			Unvisited,
			Open,
			Closed
		};

		using AStarNodeIndex = unsigned int;

		struct AStarData;

		struct AStarNode
		{
			void Score(const AStarData& aAStarData, const AStarNodeIndex aThis, const AStarNodeIndex aPrevious);
			void ScoreStart(const AStarData& aAStarData);

			[[nodiscard]] constexpr unsigned int GCost() const noexcept
			{
				return gCost;
			}

		private:

			unsigned int gCost = std::numeric_limits<unsigned int>::max(); // Distance from node to start
			unsigned int hCost = std::numeric_limits<unsigned int>::max(); // Distance from node to end
			unsigned int fCost = std::numeric_limits<unsigned int>::max(); // gCost + hCost

			static constexpr int Multiplier = 100;

		public:

			eNodeStatus status = eNodeStatus::Unvisited;

			const AStarNode* mParent = nullptr;

			[[nodiscard]] friend constexpr bool operator<(const AStarNode& a, const AStarNode& b) noexcept
			{
				if (a.fCost == b.fCost)
				{
					return a.hCost < b.hCost;
				}
				return a.fCost < b.fCost;
			}
		};

		

		struct AStarData
		{
			AStarData(const Navmesh& navmesh, const std::size_t nodeSize, const AStarNodeIndex startNodeIndex, const AStarNodeIndex endNodeIndex);

			struct NodeComparator
			{
				std::span<const AStarNode> mNodes;

				[[nodiscard]] constexpr auto operator()(const AStarNodeIndex& a, const AStarNodeIndex& b) const
				{
					return mNodes[b] < mNodes[a];
				}
			};

			const Navmesh* mNavmesh = nullptr;
			AStarNodeIndex mStartNodeIndex = GetInvalidIndex<AStarNodeIndex>();
			AStarNodeIndex mEndNodeIndex = GetInvalidIndex<AStarNodeIndex>();
			std::vector<AStarNode> mNodes;
			std::priority_queue<AStarNodeIndex, std::vector<AStarNodeIndex>, NodeComparator> mOpenSet;
		};

	public:

		NavmeshAStar() = delete;

		static std::vector<NavmeshNodeIndex> FindPath(const Navmesh& navmesh, const AStarNodeIndex start, const AStarNodeIndex end);


	private:

		static bool VisitNodes(AStarData& aStarData);

		static std::vector<NavmeshNodeIndex> BacktrackPath(AStarData& aStarData);
	};


}