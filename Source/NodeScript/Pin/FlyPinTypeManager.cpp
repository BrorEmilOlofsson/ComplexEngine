#include "FlyPinTypeManager.hpp"

namespace FLY_NAMESPACE
{

	PinTypeID PinTypeManager::CreatePinType(std::string name, const eIODirection ioDirection, const GenericDataTypeID dataTypeID, SetPinValueF setPinValueFunction, SetPinValueFromPinF setPinValueFromPinFunction, const PinTypeID parentID, const MemoryPoolID defaultValueMemoryID)
	{
		const PinTypeID id{ mPinTypes.size() };
		mPinTypes.push_back(PinType::Create(
			std::move(name),
			dataTypeID, 
			setPinValueFunction, 
			setPinValueFromPinFunction, 
			defaultValueMemoryID, 
			ioDirection,
			parentID));
		return id;
	}
}