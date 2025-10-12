#pragma once
#include <string>
#include <string_view>
#include <variant>
#include "NationalAssociation.hpp"
#include "FootballConfederation.hpp"

class FootballCompetition final
{
public:

	FootballCompetition() = default;
	FootballCompetition(std::string pName);

	[[nodiscard]] std::string_view GetName() const;
	void SetName(std::string pName);

private:

	std::string mName;
	std::variant<eNationalAssociation, eConfederation> mGoverningBody;
};