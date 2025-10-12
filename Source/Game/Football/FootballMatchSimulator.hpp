#pragma once
#include <chrono>
#include "FootballMatch.hpp"

class FootballMatchSimulator final
{
public:

	FootballMatchSimulator() = default;


private:

	std::chrono::minutes mMinute;

};

void QuickSimMatch(FootballMatch& pMatch);