#pragma once
#include <vector>
#include <span>
#include "FootballPlayerID.hpp"

class FootballTeamSquad final
{
public:

	FootballTeamSquad() = default;

	void AddPlayer(FootballPlayerID pID);
	void RemovePlayer(FootballPlayerID pID);

	[[nodiscard]] std::span<const FootballPlayerID> GetPlayers() const
	{
		return mPlayers;
	}
private:

	std::vector<FootballPlayerID> mPlayers;
};