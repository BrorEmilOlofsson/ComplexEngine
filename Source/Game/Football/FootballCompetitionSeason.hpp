#pragma once
#include "FootballSeason.hpp"
#include "FootballCompetitionID.hpp"
#include "FootballTeamID.hpp"
#include "FootballMatchID.hpp"
#include <span>
#include <vector>

class FootballCompetitionSeason final
{
public:

	FootballCompetitionSeason() = default;
	FootballCompetitionSeason(FootballSeason pSeason);

	[[nodiscard]] std::span<const FootballTeamID> GetTeams() const
	{
		return mTeams;
	}

	void AddTeam(FootballTeamID pTeamID);
	void AddMatches(std::span<FootballMatchID> pMatches);

	[[nodiscard]] std::span<const FootballMatchID> GetMatches() const
	{
		return mMatches;
	}

private:

	FootballCompetitionID mCompetitionID;
	std::vector<FootballTeamID> mTeams;
	FootballSeason mSeason;
	std::vector<FootballMatchID> mMatches;
};