#include "FlyPinType.hpp"
#include <cassert>

namespace FLY_NAMESPACE
{

	PinType PinType::Create(std::string name,
		GenericDataTypeID dataTypeID,
		SetPinValueF setPinValueFunction,
		SetPinValueFromPinF setPinValueFromPinFunction,
		MemoryPoolID defaultValueMemoryID,
		eIODirection ioDirection,
		PinTypeID parentID)
	{
		return PinType(std::move(name),
			std::move(dataTypeID),
			setPinValueFunction,
			setPinValueFromPinFunction,
			defaultValueMemoryID,
			ioDirection, 
			parentID);
    }

	PinType::PinType(std::string name, 
		GenericDataTypeID dataTypeID,
		SetPinValueF setPinValueFunction,
		SetPinValueFromPinF setPinValueFromPinFunction,
		MemoryPoolID defaultValueMemoryID, 
		eIODirection ioDirection,
		PinTypeID parentID
	)
		: mName(std::move(name))
		, mGenericDataTypeID(std::move(dataTypeID))
		, mSetPinValueFunction(setPinValueFunction)
		, mSetPinValueFromPinFunction(setPinValueFromPinFunction)
		, mDefaultValueID(defaultValueMemoryID)
		, mParentID(parentID)
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
}