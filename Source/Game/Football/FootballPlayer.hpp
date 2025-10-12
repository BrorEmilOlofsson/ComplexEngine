#pragma once
#include <string>
#include <vector>
#include <set>
#include "FootballPlayerStats.hpp"
#include "NationalAssociation.hpp"

class FootballPlayer final
{
public:

	FootballPlayer() = default;

	std::string_view GetName() const;
	void SetName(std::string pName);

	FootballPlayerStats& GetStats();
	const FootballPlayerStats& GetStats() const;

	eNationalAssociation GetMainCitizenship() const
	{
		return mMainCitizenship;
	}

	void SetMainCitizenship(eNationalAssociation pCitizenship)
	{
		if (mMainCitizenship == pCitizenship)
		{
			return;
		}
		mCitizenships.erase(mMainCitizenship);
		mMainCitizenship = pCitizenship;
		mCitizenships.insert(pCitizenship);
	}
private:

	std::string mName;
	FootballPlayerStats mStats;
	std::set<eNationalAssociation> mCitizenships;
	eNationalAssociation mMainCitizenship{};
};