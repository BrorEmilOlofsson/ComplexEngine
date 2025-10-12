#pragma once
#include <chrono>
#include <string>

class FootballSeason final
{
public:

	FootballSeason() = default;

	std::chrono::year GetStartYear() const
	{
		return mStartYear;
	}

	std::chrono::year GetEndYear() const
	{
		return mEndYear;
	}
private:

	std::chrono::year mStartYear{};
	std::chrono::year mEndYear{};
};