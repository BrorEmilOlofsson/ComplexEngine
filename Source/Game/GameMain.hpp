#pragma once
#include <memory>

namespace Simple
{
	
	class OperatingSystem;

	void RunGame(std::unique_ptr<OperatingSystem> aOperatingSystem);
}