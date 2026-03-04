#pragma once
#include "Engine/Utility/StrongType.hpp"

using FootballTeamID = CLX::StrongType<size_t, struct STFootballTeamID>;

bool Equals(FootballTeamID, FootballTeamID);

std::size_t Hash(FootballTeamID);