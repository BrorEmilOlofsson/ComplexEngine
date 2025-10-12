#pragma once
#include "../FlyDefines.hpp"
#include "../FlyRegistration.hpp"

namespace FLY_NAMESPACE
{
	std::tuple<Flow, float> Tick(NodeExecutionContext<ExecutionContextBase> context);
	Flow BeginPlay();
	Flow EndPlay();
}