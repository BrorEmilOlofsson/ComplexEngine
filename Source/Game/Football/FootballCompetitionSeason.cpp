#include "Game/Precompiled/GamePch.hpp"
#include "FootballCompetitionSeason.hpp"

FootballCompetitionSeason::FootballCompetitionSeason(FootballSeason pSeason)
	: mSeason(pSeason)
{
}

void FootballCompetitionSeason::AddTeam(FootballTeamID pTeamID)
{
	mTeams.push_back(pTeamID);
}

void FootballCompetitionSeason::AddMatches(std::span<FootballMatchID> pMatches)
{
	mMatches.insert(end(mMatches), begin(pMatches), end(pMatches));
}