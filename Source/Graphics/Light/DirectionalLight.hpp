#pragma once
#include "Utility/Math/UnitVector3.hpp"
#include "Utility/RGBColor.hpp"

namespace Simple
{

	class DirectionalLight final
	{
	public:



	public:

		UnitVector3f direction = UnitVector3f::Down();
		RGBColor color = Colors::White;
		float intensity = 1.0f;
	};
}