#include "Game/Precompiled/GamePch.hpp"
#include "FootballTeamSquad.hpp"

void FootballTeamSquad::AddPlayer(FootballPlayerID pID)
{
	mPlayers.push_back(pID);
}

void FootballTeamSquad::RemovePlayer(FootballPlayerID pID)
{
	std::erase(mPlayers, pID);
}