#pragma once
#include "Utility/Color.hpp"

namespace Simple
{
	
	struct ArrowComponent final
	{
		float length = 1.f;
		float thickness = 0.6f;
		Color color = Colors::Red;
	};
	
	void RegisterArrowComponent();
}