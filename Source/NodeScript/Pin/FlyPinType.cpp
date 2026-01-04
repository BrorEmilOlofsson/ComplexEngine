#include "FlyPinType.hpp"
#include <cassert>

namespace FLY_NAMESPACE
{
	PinType::PinType(std::string name, GenericDataTypeID dataTypeID, SetPinValueF setPinValueFunction, SetPinValueFromPinF setPinValueFromPinFunction, MemoryPoolID defaultValueMemoryID, eIODirection ioDirection)
		: mName(std::move(name))
		, mGenericDataTypeID(std::move(dataTypeID))
		, mSetPinValueFunction(setPinValueFunction)
		, mSetPinValueFromPinFunction(setPinValueFromPinFunction)
		, mDefaultValueID(defaultValueMemoryID)
		, mIODirection(ioDirection)
	{
	}

	const std::string& PinType::GetName() const
	{
		return mName;
	}

	void PinType::SetName(std::string name)
	{
		mName = std::move(name);
	}

	void PinType::AddSplitPinTypeID(PinTypeID pinTypeID)
	{
		mSplitPinTypeIDs.push_back(pinTypeID);
	}

	std::string PinIODirectionToString(const eIODirection ioDirection)
	{
		return SelectByIODirection(ioDirection, std::string("Input"), std::string("Output"));
	}

	eIODirection StringToPinIODirection(const std::string& name)
	{
		if (name == "Input")
		{
			return eIODirection::Input;
		}
		else if (name == "Output")
		{
			return eIODirection::Output;
		}
		assert(false);
		return eIODirection::Input;
	}
}