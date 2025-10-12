#pragma once
#include "Engine/Reflection/Reflection.hpp"

struct PlayerShootingComponent final
{
	float shotPower = 7000.f;
};


void RegisterPlayerShootingComponent();
