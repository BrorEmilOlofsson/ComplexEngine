#pragma once
#include "Engine/Utility/StrongType.hpp"

using FootballPlayerID = CLX::StrongType<std::size_t, struct STFootballPlayerID>;

inline bool operator==(FootballPlayerID a, FootballPlayerID b)
{
	return a.Value() == b.Value();
}