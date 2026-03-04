#include "Engine/Precompiled/EnginePch.hpp"
#include "NavmeshAStar.hpp"
#include "Navmesh.hpp"
#include "Engine/Math/ShapeMath.hpp"
#include <ranges>

#define COMMENTS_ENABLED 0

namespace Simple
{

	[[nodiscard]] constexpr NavmeshAStar::AStarNode& GetNode(NavmeshAStar::AStarData& aStarData, const NavmeshAStar::AStarNodeIndex aIndex)
	{
		return aStarData.mNodes[aIndex];
	}

	[[nodiscard]] constexpr const NavmeshAStar::AStarNode& GetNode(const NavmeshAStar::AStarData& aStarData, const NavmeshAStar::AStarNodeIndex aIndex)
	{
		return aStarData.mNodes[aIndex];
	}

	[[nodiscard]] constexpr Point2f GetPositionByIndex(const Navmesh& aNavmesh, const NavmeshAStar::AStarNodeIndex aIndex)
	{
		return aNavmesh.GetNodes()[aIndex].mCenter;
	}

	template<typename Filter>
	[[nodiscard]] static constexpr auto GetNeighboursFiltered(const NavmeshAStar::AStarData& aStarData, const NavmeshAStar::AStarNodeIndex aCurrentNodeIndex, Filter aFilter)
	{
		PROFILER_FUNCTION(profiler::colors::DeepOrange900);

		const NavmeshNode& currentNode = aStarData.mNavmesh->GetNodes()[aCurrentNodeIndex];

		auto filter = [aFilter, &aStarData](const NavmeshNodeIndex& aNodeIndex) -> bool
			{
				if (aNodeIndex == GetInvalidIndex<NavmeshNodeIndex>())
				{
					return false;
				}

				if (aStarData.mNavmesh->GetNodes()[aNodeIndex].mState == eNavmeshNodeState::Unwalkable)
				{
					return false;
				}

				if constexpr (std::same_as<Filter, std::nullptr_t>)
				{
					return true;
				}
				else
				{
					return aFilter(GetNode(aStarData, aNodeIndex));
				}
			};


		return currentNode.mConnections | std::views::filter(filter);
	}

	[[nodiscard]] static constexpr auto GetNeighbours(const NavmeshAStar::AStarData& aStarData, const NavmeshAStar::AStarNodeIndex aNodeIndex)
	{
		return GetNeighboursFiltered(aStarData, aNodeIndex, nullptr);
	}


	[[nodiscard]] static bool IsNodeNotClosed(const NavmeshAStar::AStarNode& node)
	{
		return node.status != NavmeshAStar::eNodeStatus::Closed;
	}

	static void ScoreNeighbours(NavmeshAStar::AStarData& aStarData, const NavmeshAStar::AStarNodeIndex currentNodeIndex)
	{
		PROFILER_FUNCTION(profiler::colors::Brown700);
		for (const NavmeshAStar::AStarNodeIndex neighbourIndex : GetNeighboursFiltered(aStarData, currentNodeIndex, IsNodeNotClosed))
		{
			NavmeshAStar::AStarNode& neighbour = GetNode(aStarData, neighbourIndex);
			neighbour.status = NavmeshAStar::eNodeStatus::Open;

			neighbour.Score(aStarData, neighbourIndex, currentNodeIndex);

			PROFILER_BEGIN("Added to open set");
			aStarData.mOpenSet.push(neighbourIndex);
			PROFILER_END();
		}
	}

	void NavmeshAStar::AStarNode::Score(const AStarData& aStarData, const AStarNodeIndex self, const AStarNodeIndex previous)
	{
		PROFILER_FUNCTION(profiler::colors::Blue600);
		const Point2f thisPos = Lerp(aStarData.mNavmesh->GetEdgeLineSegment2DFromNodes(previous, self).value(), 0.5f);
		const Point2f& previousPos = GetPositionByIndex(*aStarData.mNavmesh, previous);
		const Point2f& endPos = GetPositionByIndex(*aStarData.mNavmesh, aStarData.mEndNodeIndex);

		const int unsigned newGCost = static_cast<unsigned int>(Distance(previousPos, thisPos) * Multiplier + GetNode(aStarData, previous).gCost);

		const unsigned int newHCost = static_cast<unsigned int>(Distance(endPos, thisPos) * Multiplier);

		const unsigned int newFCost = newGCost + newHCost;

		if (newFCost < fCost)
		{
			gCost = newGCost;
			hCost = newHCost;
			fCost = newFCost;

			mParent = &GetNode(aStarData, previous);
		}
	}

	void NavmeshAStar::AStarNode::ScoreStart(const AStarData& aStarData)
	{
		const Point2f& startPos = GetPositionByIndex(*aStarData.mNavmesh, aStarData.mStartNodeIndex);
		const Point2f& endPos = GetPositionByIndex(*aStarData.mNavmesh, aStarData.mEndNodeIndex);

		gCost = 0;

		hCost = static_cast<unsigned int>(Distance(startPos, endPos) * Multiplier);

		fCost = gCost + hCost;

		status = eNodeStatus::Closed;
	}

	NavmeshAStar::AStarData::AStarData(const Navmesh& navmesh, const std::size_t nodeSize, const AStarNodeIndex startNodeIndex, const AStarNodeIndex endNodeIndex)
		: mNavmesh(&navmesh)
		, mNodes(nodeSize)
		, mStartNodeIndex(startNodeIndex)
		, mEndNodeIndex(endNodeIndex)
		, mOpenSet(NodeComparator{ mNodes })
	{
	}

	std::vector<NavmeshNodeIndex> NavmeshAStar::FindPath(const Navmesh& navmesh, const AStarNodeIndex start, const AStarNodeIndex end)
	{
		PROFILER_FUNCTION(profiler::colors::Yellow300);
		PROFILER_BEGIN("Init A* Data");
		AStarData aStarData(navmesh, navmesh.GetNodes().size(), start, end);
		PROFILER_END();
		GetNode(aStarData, start).ScoreStart(aStarData);

		const bool succeded = VisitNodes(aStarData);

		if (!succeded)
		{
			return std::vector<NavmeshNodeIndex>();
		}

		return BacktrackPath(aStarData);
	}

	bool NavmeshAStar::VisitNodes(AStarData& aStarData)
	{
		PROFILER_FUNCTION(profiler::colors::DeepOrange50);
		auto& openSet = aStarData.mOpenSet;
		AStarNodeIndex currentIndex = GetInvalidIndex<AStarNodeIndex>();
		openSet.push(aStarData.mStartNodeIndex);

		while (!openSet.empty())
		{
			PROFILER_BEGIN("Visit Nodes Iteration");
			currentIndex = openSet.top();
			if (currentIndex == aStarData.mEndNodeIndex)
			{
				return true;
			}

			GetNode(aStarData, currentIndex).status = eNodeStatus::Closed;
			openSet.pop();
			ScoreNeighbours(aStarData, currentIndex);


			PROFILER_END();
		}
		return false;
	}

	std::vector<NavmeshNodeIndex> NavmeshAStar::BacktrackPath(AStarData& aStarData)
	{
		PROFILER_FUNCTION(profiler::colors::Grey300);
		std::vector<NavmeshNodeIndex> reversePath;

		AStarNodeIndex currentNodeIndex = aStarData.mEndNodeIndex;
		bool isDone = false;
		while (!isDone)
		{
			reversePath.push_back(currentNodeIndex);
			AStarNodeIndex closestNeighbourIndex = GetInvalidIndex<AStarNodeIndex>();
#if COMMENTS_ENABLED
			std::cout << "Current Node: " << currentNode->mNavmeshNodeIndex << std::endl;
#endif
			PROFILER_BEGIN("Iterate neighbours");
			for (const AStarNodeIndex neighbourIndex : GetNeighbours(aStarData, currentNodeIndex))
			{
#if COMMENTS_ENABLED
				std::cout << "GCOST of node " << neighbour->mNavmeshNodeIndex << ": " << neighbour->gCost << std::endl;
#endif
				if (neighbourIndex == aStarData.mStartNodeIndex)
				{
					reversePath.push_back(neighbourIndex);
					isDone = true;
					break;
				}
				AStarNode& neighbour = GetNode(aStarData, neighbourIndex);
				if (neighbour.status == eNodeStatus::Closed)
				{
					if (closestNeighbourIndex == GetInvalidIndex<AStarNodeIndex>() || neighbour.GCost() <= GetNode(aStarData, closestNeighbourIndex).GCost())
					{
						neighbour.status = eNodeStatus::Unvisited;
						closestNeighbourIndex = neighbourIndex;
					}
				}
			}
			PROFILER_END();
			currentNodeIndex = closestNeighbourIndex;
		}

		PROFILER_BEGIN("Reverse path");
		std::ranges::reverse(reversePath);
		PROFILER_END();

		return reversePath;
	}
}