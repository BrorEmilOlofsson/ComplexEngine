#include "Game/Precompiled/GamePch.hpp"
#include "FootballMatch.hpp"

FootballMatch::FootballMatch(FootballTeamID pTeamID1, FootballTeamID pTeamID2)
	: mTeam1ID(pTeamID1)
	, mTeam2ID(pTeamID2)
{
}

FootballTeamID FootballMatch::GetTeam1ID() const
{
	return mTeam1ID;
}

FootballTeamID FootballMatch::GetTeam2ID() const
{
	return mTeam2ID;
}

FootballMatchScore& FootballMatch::GetScore()
{
	return mScore;
}

const FootballMatchScore& FootballMatch::GetScore() const
{
	return mScore;
}