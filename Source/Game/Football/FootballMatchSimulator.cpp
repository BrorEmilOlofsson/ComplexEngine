#include "Game/Precompiled/GamePch.hpp"
#include "FootballMatchSimulator.hpp"
#include "Probability.hpp"

void QuickSimMatch(FootballMatch& pMatch)
{
	pMatch.GetScore().AddGoalForTeam1(ScoredGoal());
	pMatch.GetScore().AddGoalForTeam2(ScoredGoal());

}