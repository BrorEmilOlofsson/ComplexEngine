#include "Game/Precompiled/GamePch.hpp"
#include "FootballCompetition.hpp"

FootballCompetition::FootballCompetition(std::string pName)
	: mName(std::move(pName))
{
}

std::string_view FootballCompetition::GetName() const
{
	return mName;
}

void FootballCompetition::SetName(std::string pName)
{
	mName = std::move(pName);
}