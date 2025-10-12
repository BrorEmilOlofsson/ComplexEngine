#include "Game/Precompiled/GamePch.hpp"
#include "FootballMatchScore.hpp"

FootballMatchScore::FootballMatchScore(std::vector<ScoredGoal> pTeam1Goals, std::vector<ScoredGoal> pTeam2Goals)
	: mTeam1Goals(std::move(pTeam1Goals))
	, mTeam2Goals(std::move(pTeam2Goals))
{
}

void FootballMatchScore::AddGoalForTeam1(ScoredGoal pGoal)
{
	mTeam1Goals.push_back(std::move(pGoal));
}

void FootballMatchScore::AddGoalForTeam2(ScoredGoal pGoal)
{
	mTeam2Goals.push_back(std::move(pGoal));
}

template<typename T>
static constexpr eFootballMatchResult GetResultInternal(T pGoalCount1, T pGoalCount2)
{
	return pGoalCount1 > pGoalCount2 ? eFootballMatchResult::Win : pGoalCount1 == pGoalCount2 ? eFootballMatchResult::Draw : eFootballMatchResult::Loss;
}

eFootballMatchResult FootballMatchScore::GetTeam1Result() const
{
	return GetResultInternal(GetTeam1GoalCount(), GetTeam2GoalCount());
}

eFootballMatchResult FootballMatchScore::GetTeam2Result() const
{
	return GetResultInternal(GetTeam2GoalCount(), GetTeam1GoalCount());
}

std::size_t FootballMatchScore::GetTeam1GoalCount() const
{
	return mTeam1Goals.size();
}

std::size_t FootballMatchScore::GetTeam2GoalCount() const
{
	return mTeam2Goals.size();
}