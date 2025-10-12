#pragma once
#include <string>

enum class eConfederation
{
	AFC,
	CAF,
	CONCACAF,
	CONMEBOL,
	OFC,
	UEFA,
};

constexpr std::string EnumToString(eConfederation pConfederation)
{
	switch (pConfederation)
	{
	case eConfederation::AFC: return "AFC";
	case eConfederation::CAF: return "CAF";
	case eConfederation::CONCACAF: return "CONCACAF";
	case eConfederation::CONMEBOL: return "CONMEBOL";
	case eConfederation::OFC: return "OFC";
	case eConfederation::UEFA: return "UEFA";
	default: return "Unknown";
	}
}