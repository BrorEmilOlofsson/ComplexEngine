#pragma once
#include "../Utilities/FlyFunctional.hpp"

namespace FLY_NAMESPACE
{

	struct SetPinValueData;
	struct SetPinValueFromPinData;
	struct InternalExecutionContext;

	using SetPinValueF = FunctionWrapper<void(const SetPinValueData&, const InternalExecutionContext&), struct SetPinValueParam>;
	using SetPinValueFromPinF = FunctionWrapper<void(const SetPinValueFromPinData&, const InternalExecutionContext&), struct SetPinValueFromPinParam>;

}