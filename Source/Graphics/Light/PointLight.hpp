#pragma once
#include "Utility/Math/Point3.hpp"
#include "Utility/RGBColor.hpp"

namespace Simple
{

	struct PointLight final
	{
		Point3f position;
		RGBColor color = Colors::White;
		float intensity = 1.f;
		float range = 10.f;
	};
}