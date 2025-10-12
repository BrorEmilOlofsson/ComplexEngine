#pragma once


struct PlanetComponent final
{
	float radius = 3;
	float gravityMagnitude = 9.81f;
};

void RegisterPlanetComponent();
