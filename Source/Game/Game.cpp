#include "Game/Precompiled/GamePch.hpp"
#include "Game/Game.hpp"
#include "Football/FootballTeam.hpp"
#include "Football/FootballCompetition.hpp"
#include "Football/FootballCompetitionSeason.hpp"
#include "Football/FootballPlayer.hpp"
#include "Football/FootballMatch.hpp"
#include "Football/FootballPlayerGenerator.hpp"
#include "Football/FootballMatchFormatGenerator.hpp"
#include "Football/FootballMatchSimulator.hpp"
#include "Engine/Physics/Physics.hpp"
#include "Engine/Physics/PhysicsObject.hpp"
#include "Game/ECS/GameSystemRegistration.hpp"

namespace Simple
{
	Game::Game()
	{	/*auto teams = MakeTeams();
		const auto players = GeneratePlayers(teams);
		std::vector<FootballMatch> matches;

		FootballCompetition premierLeague;
		FootballCompetitionSeason premierLeague2425;

		for (size_t i = 0; i < size(teams); i++)
		{
			premierLeague2425.AddTeam(FootballTeamID(i));
		}

		premierLeague.SetName("Premier League");

		std::chrono::time_point t1 = std::chrono::high_resolution_clock::now();
		auto pairings = GenerateLeaguePairings(premierLeague2425);
		std::chrono::time_point t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> d = t2 - t1;
		std::cout << d << std::endl;
		pairings;

		for (size_t i = 0; i < size(pairings); i++)
		{
			matches.push_back(FootballMatch(pairings[i].first, pairings[i].second));
		}

		std::vector<FootballMatchID> matchIDs;
		matchIDs.reserve(size(matches));

		for (size_t i = 0; i < size(matches); i++)
		{
			matchIDs.push_back(FootballMatchID(i));
		}
		premierLeague2425.AddMatches(matchIDs);

		for (FootballMatch& match : matches)
		{
			QuickSimMatch(match);
		}

		PrintSquads(teams, players);

		const FootballTable table = CreateTable(premierLeague2425, matches);
		PrintTable(table, teams);

		std::optional<double> t = Physics::CalculateTheta<double, eQuadraticReturn::Min>(Physics::Speed(28.69), 36.21, 2.3, 0.11);

		std::cout << t.value() << std::endl;
		t;*/



	}


	std::vector<FootballTeam> MakeTeams()
	{
		std::vector<FootballTeam> teams;
		teams.push_back(FootballTeam("Ipswich"));
		teams.push_back(FootballTeam("West Ham"));
		teams.push_back(FootballTeam("Arsenal"));
		teams.push_back(FootballTeam("Manchester United"));
		teams.push_back(FootballTeam("Chelsea"));
		teams.push_back(FootballTeam("Manchester City"));
		teams.push_back(FootballTeam("Tottenham Hotspur"));
		teams.push_back(FootballTeam("Liverpool"));
		teams.push_back(FootballTeam("Southampton"));
		teams.push_back(FootballTeam("Fulham"));
		teams.push_back(FootballTeam("Newcastle United"));
		teams.push_back(FootballTeam("Brighton"));
		teams.push_back(FootballTeam("Aston Villa"));
		teams.push_back(FootballTeam("Bournemouth"));
		teams.push_back(FootballTeam("Nottingham Forest"));
		teams.push_back(FootballTeam("Leicester City"));
		teams.push_back(FootballTeam("Brentford"));
		teams.push_back(FootballTeam("Everton"));
		teams.push_back(FootballTeam("Wolverhampton"));
		teams.push_back(FootballTeam("Crystal Palace"));

		return teams;
	}

	std::size_t ResultToPoints(const eFootballMatchResult pResult)
	{
		switch (pResult)
		{
		case eFootballMatchResult::Win: return 3;
		case eFootballMatchResult::Draw: return 1;
		case eFootballMatchResult::Loss: return 0;
		default: return 0;
		}
	}

	struct TeamTableData
	{
		FootballTeamID mTeamID;
		std::size_t mPoints = 0;
		std::size_t mWins = 0;
		std::size_t mDraws = 0;
		std::size_t mLosses = 0;
		std::size_t mMatchesPlayed = 0;
		std::size_t mGoalsScored = 0;
	};


	class FootballTable final
	{
	public:

		FootballTable() = default;
		FootballTable(const std::size_t pTeamCount)
			: mData(pTeamCount)
		{
		}

		constexpr void SetTeamCount(const std::size_t pTeamCount)
		{
			mData.resize(pTeamCount);
		}

		constexpr void Clear()
		{
			const std::size_t teamCount = GetTeamCount();
			mData.clear();
			SetTeamCount(teamCount);
		}

		[[nodiscard]] constexpr TeamTableData& operator[](const std::size_t pIndex)
		{
			return mData[pIndex];
		}

		[[nodiscard]] constexpr const TeamTableData& operator[](const std::size_t pIndex) const
		{
			return mData[pIndex];
		}

		constexpr std::size_t GetTeamCount() const
		{
			return mData.size();
		}

		constexpr std::vector<TeamTableData>::iterator begin()
		{
			return mData.begin();
		}

		constexpr std::vector<TeamTableData>::iterator end()
		{
			return mData.end();
		}

		constexpr std::vector<TeamTableData>::const_iterator begin() const
		{
			return mData.begin();
		}

		constexpr std::vector<TeamTableData>::const_iterator end() const
		{
			return mData.end();
		}

	private:

		std::vector<TeamTableData> mData;
	};

	static bool StandardTiebreakerSort(const TeamTableData& pA, const TeamTableData& pB)
	{
		if (pA.mPoints == pB.mPoints)
		{
			return pA.mGoalsScored > pB.mGoalsScored;
		}
		return pA.mPoints > pB.mPoints;
	}

	struct StandardTiebreakerSorter final
	{
		bool operator()(const TeamTableData& pA, const TeamTableData& pB) const
		{
			return StandardTiebreakerSort(pA, pB);
		}
	};

	template<typename SortPredicate = StandardTiebreakerSorter>
	FootballTable CreateTable(const FootballCompetitionSeason& pCompetitionSeason, const std::span<const FootballMatch> pMatches, SortPredicate&& pSortPredicate = {})
	{
		const auto competitionMatches = pCompetitionSeason.GetMatches();

		FootballTable table(size(pCompetitionSeason.GetTeams()));

		std::unordered_map<FootballTeamID, std::size_t> idMap;
		for (size_t i = 0; i < size(pCompetitionSeason.GetTeams()); i++)
		{
			idMap.emplace(pCompetitionSeason.GetTeams()[i], i);
			table[i].mTeamID = pCompetitionSeason.GetTeams()[i];
		}

		auto addDataPerMatch = [](TeamTableData& pTeamTableData, const eFootballMatchResult pResult, const std::size_t pGoals)
			{
				pTeamTableData.mMatchesPlayed++;
				pTeamTableData.mPoints += ResultToPoints(pResult);
				pTeamTableData.mGoalsScored += pGoals;

				switch (pResult)
				{
				case eFootballMatchResult::Win:
					++pTeamTableData.mWins;
					break;
				case eFootballMatchResult::Draw:
					++pTeamTableData.mDraws;
					break;
				case eFootballMatchResult::Loss:
					++pTeamTableData.mLosses;
					break;
				default:
					break;
				}
			};

		for (const FootballMatchID footballMatchID : competitionMatches)
		{
			const FootballMatch& match = pMatches[footballMatchID.Value()];

			TeamTableData& team1Data = table[idMap.at(match.GetTeam1ID())];
			TeamTableData& team2Data = table[idMap.at(match.GetTeam2ID())];

			addDataPerMatch(team1Data, match.GetScore().GetTeam1Result(), match.GetScore().GetTeam1GoalCount());
			addDataPerMatch(team2Data, match.GetScore().GetTeam2Result(), match.GetScore().GetTeam2GoalCount());
		}

		std::sort(table.begin(), table.end(), pSortPredicate);

		return table;
	}

	void PrintTable(const FootballTable& pTable, const std::span<const FootballTeam> pTeams)
	{
		std::cout << "Name | " << "MP" << " | " << "P" << " | " << "GS" << std::endl;
		for (auto& teamTableData : pTable)
		{
			std::cout << pTeams[teamTableData.mTeamID.Value()].GetName() << "\t | " << teamTableData.mMatchesPlayed <<
				" | " << teamTableData.mPoints << " | " << teamTableData.mGoalsScored << "\n";
		}
	}

	std::vector<FootballPlayer> GeneratePlayers(std::vector<FootballTeam>& pTeams)
	{
		std::vector<FootballPlayer> players;

		for (size_t i = 0; i < size(pTeams); i++)
		{
			for (size_t j = 0; j < 24; j++)
			{
				players.push_back(GenerateFootballPlayer());
				pTeams[i].GetSquad().AddPlayer(FootballPlayerID(i * 24 + j));
			}
		}

		return players;
	}

	void PrintPlayer(const FootballPlayer& pPlayer)
	{
		std::cout << pPlayer.GetName() << ", " << EnumToString(pPlayer.GetMainCitizenship()) << "\n";
	}

	void PrintSquad(const FootballTeamSquad& pSquad, const std::span<const FootballPlayer> pPlayers)
	{
		for (auto& playerID : pSquad.GetPlayers())
		{
			PrintPlayer(pPlayers[playerID.Value()]);
		}
	}

	void PrintSquads(const std::span<const FootballTeam> pTeams, const std::span<const FootballPlayer> pPlayers)
	{
		for (auto& team : pTeams)
		{
			PrintSquad(team.GetSquad(), pPlayers);

			std::cout << std::endl;
		}
	}

}