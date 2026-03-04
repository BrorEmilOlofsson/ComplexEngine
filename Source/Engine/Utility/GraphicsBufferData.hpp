#pragma once
#include "Engine/Utility/SystemTimer.hpp"

namespace CLX
{
	struct GraphicsBufferData final
	{
		SystemTimerf frameTimer;
		SystemTimerd totalTimer;
	};
}