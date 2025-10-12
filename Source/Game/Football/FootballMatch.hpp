#pragma once
#include "FootballTeamID.hpp"
#include "FootballMatchScore.hpp"

class FootballMatch final
{
public:

	FootballMatch() = default;
	FootballMatch(FootballTeamID pTeamID1, FootballTeamID pTeamID2);

	[[nodiscard]] FootballTeamID GetTeam1ID() const;
	[[nodiscard]] FootballTeamID GetTeam2ID() const;

	[[nodiscard]] FootballMatchScore& GetScore();
	[[nodiscard]] const FootballMatchScore& GetScore() const;

private:

	FootballTeamID mTeam1ID;
	FootballTeamID mTeam2ID;
	FootballMatchScore mScore;
	std::chrono::day mDay{};
};