#pragma once
#include "Engine/Utility/Color.hpp"

namespace CLX
{
	
	struct ArrowComponent final
	{
		float length = 1.f;
		float thickness = 0.6f;
		Color color = Colors::Red;
	};
	
	void RegisterArrowComponent();
}