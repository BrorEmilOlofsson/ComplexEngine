#pragma once
#include <string>

enum class eFootballMatchResult : unsigned char
{
	Win,
	Draw,
	Loss
};

constexpr std::string EnumToString(eFootballMatchResult aResult)
{
	switch (aResult)
	{
	case eFootballMatchResult::Win: return "Win";
	case eFootballMatchResult::Draw: return "Draw";
	case eFootballMatchResult::Loss: return "Loss";
	default: return "Unknown";
	}
}