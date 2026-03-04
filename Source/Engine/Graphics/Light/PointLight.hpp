#pragma once
#include "Engine/Math/Point3.hpp"
#include "Engine/Utility/RGBColor.hpp"

namespace CLX
{

	struct PointLight final
	{
		Point3f position;
		RGBColor color = Colors::White;
		float intensity = 1.f;
		float range = 10.f;
	};
}