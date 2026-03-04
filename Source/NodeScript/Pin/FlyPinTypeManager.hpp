#pragma once
#include "../FlyDefines.hpp"
#include "FlyPinType.hpp"
#include <string>
#include <vector>

namespace FLY_NAMESPACE
{
	class PinTypeManager final
	{
	public:

		PinTypeManager() = default;

		template<typename T>
		[[nodiscard]] PinTypeID CreatePinType(std::string name, eIODirection ioDirection, SetPinValueF setPinValueFunction, SetPinValueFromPinF setPinValueFromPinFunction, PinTypeID parentID, MemoryPoolID defaultValueMemoryID = InvalidID<MemoryPoolID>())
		{
			return CreatePinType(name, ioDirection, GenericDataTypeID{ GetDataTypeID<T>() }, setPinValueFunction, setPinValueFromPinFunction, parentID, defaultValueMemoryID);
		}

		[[nodiscard]] PinTypeID CreatePinType(std::string name, eIODirection ioDirection, GenericDataTypeID dataTypeID, SetPinValueF setPinValueFunction, SetPinValueFromPinF setPinValueFromPinFunction, PinTypeID parentID, MemoryPoolID defaultValueMemoryID = InvalidID<MemoryPoolID>());
		
		[[nodiscard]] constexpr PinType& GetPinType(PinTypeID id)
		{
			return mPinTypes[id];
		}

		[[nodiscard]] constexpr const PinType& GetPinType(PinTypeID id) const
		{
			return mPinTypes[id];
		}

	private:
		std::vector<PinType> mPinTypes;
	};
}