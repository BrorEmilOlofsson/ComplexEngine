#pragma once
#include "FootballPlayerID.hpp"
#include <optional>
#include <chrono>

class ScoredGoal final
{
public:

	ScoredGoal() = default;
	ScoredGoal(std::chrono::minutes pMinute, FootballPlayerID pScorer, std::optional<FootballPlayerID> pAssister = std::nullopt);

	FootballPlayerID GetScorer() const;
	std::optional<FootballPlayerID> GetAssister() const;

	std::chrono::minutes GetMinute() const
	{
		return mMinute;
	}

private:

	FootballPlayerID mScorer;
	std::optional<FootballPlayerID> mAssister;
	std::chrono::minutes mMinute{};
};