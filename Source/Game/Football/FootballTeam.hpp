#pragma once
#include <string>
#include <string_view>
#include "FootballTeamSquad.hpp"

class FootballTeam final
{
public:

	FootballTeam() = default;
	explicit FootballTeam(std::string pName);

	void SetName(std::string pName);
	[[nodiscard]] std::string_view GetName() const;

	[[nodiscard]] FootballTeamSquad& GetSquad();
	[[nodiscard]] const FootballTeamSquad& GetSquad() const;

private:

	FootballTeamSquad mSquad;
	std::string mName;
};