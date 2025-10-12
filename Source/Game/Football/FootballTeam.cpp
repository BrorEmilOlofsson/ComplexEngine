#include "Game/Precompiled/GamePch.hpp"
#include "FootballTeam.hpp"

FootballTeam::FootballTeam(std::string pName)
	: mName(std::move(pName))
{
}

void FootballTeam::SetName(std::string pName)
{
	mName = std::move(pName);
}

std::string_view FootballTeam::GetName() const
{
	return mName;
}

FootballTeamSquad& FootballTeam::GetSquad()
{
	return mSquad;
}

const FootballTeamSquad& FootballTeam::GetSquad() const
{
	return mSquad;
}