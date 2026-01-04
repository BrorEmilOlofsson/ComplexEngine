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

		PinTypeManager();
		~PinTypeManager();

		//PinTypeID CreatePinType(std::string_view aName, eFlowType aFlowType, DataTypeID aDataTypeID, SetPinValueFunction aSetPinValueFunction, SetPinValueFromPinInterface aSetPinValueFromPinInterface, MemoryPoolID aDefaultValueMemoryID = InvalidID<MemoryPoolID>());

		template<typename T>
		[[nodiscard]] PinTypeID CreatePinType(std::string name, eIODirection ioDirection, SetPinValueF setPinValueFunction, SetPinValueFromPinF setPinValueFromPinFunction, MemoryPoolID defaultValueMemoryID = InvalidID<MemoryPoolID>())
		{
			return CreatePinType(name, ioDirection, GenericDataTypeID{ GetDataTypeID<T>() }, setPinValueFunction, setPinValueFromPinFunction, defaultValueMemoryID);
		}

		[[nodiscard]] PinTypeID CreatePinType(std::string name, eIODirection ioDirection, GenericDataTypeID dataTypeID, SetPinValueF setPinValueFunction, SetPinValueFromPinF setPinValueFromPinFunction, MemoryPoolID defaultValueMemoryID = InvalidID<MemoryPoolID>());

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