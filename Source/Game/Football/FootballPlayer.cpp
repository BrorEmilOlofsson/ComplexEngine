#include "Game/Precompiled/GamePch.hpp"
#include "FootballPlayer.hpp"

std::string_view FootballPlayer::GetName() const
{
	return mName;
}

void FootballPlayer::SetName(std::string pName)
{
	mName = std::move(pName);
}

FootballPlayerStats& FootballPlayer::GetStats()
{
	return mStats;
}

const FootballPlayerStats& FootballPlayer::GetStats() const
{
	return mStats;
}