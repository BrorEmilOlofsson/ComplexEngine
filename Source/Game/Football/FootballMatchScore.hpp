#pragma once
#include "FootballMatchResult.hpp"
#include "ScoredGoal.hpp"
#include <vector>
#include <span>

class FootballMatchScore final
{
public:

	FootballMatchScore() = default;
	FootballMatchScore(std::vector<ScoredGoal> pTeam1Goals, std::vector<ScoredGoal> pTeam2Goals);

	void AddGoalForTeam1(ScoredGoal pGoal);
	void AddGoalForTeam2(ScoredGoal pGoal);

	[[nodiscard]] eFootballMatchResult GetTeam1Result() const;
	[[nodiscard]] eFootballMatchResult GetTeam2Result() const;

	[[nodiscard]] std::size_t GetTeam1GoalCount() const;
	[[nodiscard]] std::size_t GetTeam2GoalCount() const;

	[[nodiscard]] std::span<const ScoredGoal> GetTeam1Goals() const
	{
		return mTeam1Goals;
	}

	[[nodiscard]] std::span<const ScoredGoal> GetTeam2Goals() const
	{
		return mTeam2Goals;
	}

private:

	std::vector<ScoredGoal> mTeam1Goals;
	std::vector<ScoredGoal> mTeam2Goals;
};