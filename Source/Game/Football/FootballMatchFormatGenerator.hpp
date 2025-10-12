#pragma once
#include "FootballTeamID.hpp"
#include "FootballCompetitionSeason.hpp"
#include <vector>
#include <utility>
#include <random>
#include <numeric>

std::vector<std::size_t> RoundRobin1(const std::size_t pCount)
{
	const std::size_t teamCount = pCount;
	std::vector<std::size_t> indices(teamCount);
	std::iota(begin(indices), end(indices), 0);

	{
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(begin(indices), end(indices), g);
	}

	const std::size_t matchWeekCount = teamCount - 1;
	const std::size_t matchCount = (teamCount * matchWeekCount) / 2;

	std::vector<std::size_t> result;
	result.reserve(matchCount * 2);
	for (std::size_t i = 0; i < matchWeekCount; i++)
	{
		for (std::size_t j = 0; j < teamCount / 2; j++)
		{
			result.push_back(indices[j]);
			result.push_back(indices[teamCount - j - 1]);
		}

		std::rotate(begin(indices) + 1, end(indices) - 1, end(indices));
	}
	{
		std::random_device rd;
		std::mt19937 g(rd());

		const std::size_t matchCountPerMatchweek = teamCount / 2;
		for (size_t i = 0; i < matchWeekCount; i++)
		{
			std::shuffle(begin(result) + i * matchCountPerMatchweek * 2, begin(result) + i * matchCountPerMatchweek * 2 + matchCountPerMatchweek, g);
		}
	}

	return result;
}

std::vector<std::pair<std::size_t, std::size_t>> RoundRobin(const std::size_t pCount)
{
	const std::size_t teamCount = pCount;
	std::vector<std::size_t> indices(teamCount);
	std::iota(begin(indices), end(indices), 0);

	{
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(begin(indices), end(indices), g);
	}

	const std::size_t matchWeekCount = teamCount - 1;
	const std::size_t matchCount = (teamCount * matchWeekCount);

	std::vector<std::pair<std::size_t, std::size_t>> result;
	result.reserve(matchCount);
	for (std::size_t i = 0; i < matchWeekCount; i++)
	{
		for (std::size_t j = 0; j < teamCount / 2; j++)
		{
			result.push_back(std::make_pair(indices[j], indices[teamCount - j - 1]));
		}

		std::rotate(begin(indices) + 1, end(indices) - 1, end(indices));
	}
	{
		std::random_device rd;
		std::mt19937 g(rd());

		const std::size_t matchCountPerMatchweek = teamCount / 2;
		for (size_t i = 0; i < matchWeekCount; i++)
		{
			std::shuffle(begin(result) + i * matchCountPerMatchweek, begin(result) + i * matchCountPerMatchweek + matchCountPerMatchweek, g);
		}
	}

	result.insert(begin(result) + size(result), begin(result), end(result));

	std::for_each(begin(result) + matchCount / 2, end(result), [](auto& pPair) { std::swap(pPair.first, pPair.second); });

	return result;
}

[[nodiscard]] std::vector<std::pair<FootballTeamID, FootballTeamID>> GenerateLeaguePairings(const FootballCompetitionSeason& pCompetition)
{
	const std::span<const FootballTeamID> teams = pCompetition.GetTeams();
	const std::size_t teamCount = size(teams);

	const std::vector<std::pair<std::size_t, std::size_t>> schedule = RoundRobin(teamCount);

	std::vector<std::pair<FootballTeamID, FootballTeamID>> result(size(schedule));

	std::ranges::transform(schedule, std::back_inserter(result),
		[&teams](const std::pair<std::size_t, std::size_t>& pPair)
		{
			return std::make_pair(teams[pPair.first], teams[pPair.second]);
		}
	);

	return result;
}