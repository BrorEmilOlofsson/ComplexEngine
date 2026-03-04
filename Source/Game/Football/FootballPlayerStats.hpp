#pragma once
#include "Engine/Utility/StrongType.hpp"
#include "Height.hpp"
#include "Engine/Utility/Clamper.hpp"

using FootballPlayerStat = CLX::StrongCheckedType<float, CLX::Clamper_f<0.f, 100.f>, struct STFootballPlayerStat>;

FootballPlayerStat Add(FootballPlayerStat a, FootballPlayerStat b);
FootballPlayerStat Subtract(FootballPlayerStat a, FootballPlayerStat b);
FootballPlayerStat& AddEqual(FootballPlayerStat& a, FootballPlayerStat b);
FootballPlayerStat& SubtractEqual(FootballPlayerStat& a, FootballPlayerStat b);

class FootballPlayerStats final
{
public:

	FootballPlayerStats() = default;

	FootballPlayerStat mStamina;
	FootballPlayerStat mShotPower;

	Height mHeight;
};