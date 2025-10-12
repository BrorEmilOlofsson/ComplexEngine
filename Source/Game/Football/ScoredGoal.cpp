#include "Game/Precompiled/GamePch.hpp"
#include "ScoredGoal.hpp"

ScoredGoal::ScoredGoal(std::chrono::minutes pMinute, FootballPlayerID pScorer, std::optional<FootballPlayerID> pAssister)
	: mScorer(pScorer)
	, mAssister(pAssister)
	, mMinute(pMinute)
{
}

FootballPlayerID ScoredGoal::GetScorer() const
{
	return mScorer;
}

std::optional<FootballPlayerID> ScoredGoal::GetAssister() const
{
	return mAssister;
}