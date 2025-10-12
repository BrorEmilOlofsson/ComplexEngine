#pragma once
#include "Utility/SystemTimer.hpp"

namespace Simple
{
	struct GraphicsBufferData final
	{
		SystemTimerf frameTimer;
		SystemTimerd totalTimer;
	};
}