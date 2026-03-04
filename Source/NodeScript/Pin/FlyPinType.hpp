#pragma once
#include <string>
#include <vector>

#include "../FlyDefines.hpp"
#include "../Graph/FlyNodeGraphVariant.hpp"
#include "../Pin/FlyIODirection.hpp"
#include "../Pin/FlySetPinValueFunctions.hpp"

namespace FLY_NAMESPACE
{

	class NodeGraph;

	struct SetPinValueData final
	{
		NodeGraph* mNodeGraph = nullptr;
		NodeGraphVariantHandle mNodeGraphVariantHandle;
		const void* mReadFromDataPtr = nullptr;
		PinID mWriteToPinID;
#ifdef FLY_DEBUG
		GenericDataTypeID mReadFromDataTypeID;
#endif
	};

	struct SetPinValueFromPinData final
	{
		NodeGraph* mWriteToPinNodeGraph = nullptr;
		const NodeGraph* mReadFromPinNodeGraph = nullptr;
		PinID mWriteToPinID;
		PinID mReadFromPinID;
	};

	class PinType final
	{
	public:

		
		[[nodiscard]] static PinType Create(std::string name,
			GenericDataTypeID dataTypeID,
			SetPinValueF setPinValueFunction,
			SetPinValueFromPinF setPinValueFromPinFunction,
			MemoryPoolID defaultValueMemoryID,
			eIODirection ioDirection,
			PinTypeID parentID);

		[[nodiscard]] const std::string& GetName() const;
		[[nodiscard]] constexpr GenericDataTypeID GetDataTypeID() const;
		[[nodiscard]] constexpr SetPinValueF GetSetPinValueFunction() const;
		[[nodiscard]] constexpr SetPinValueFromPinF GetSetPinValueFromPinFunction() const;
		[[nodiscard]] constexpr MemoryPoolID GetDefaultValueMemoryID() const;
		[[nodiscard]] constexpr const std::vector<PinTypeID>& GetSplitPinTypeIDs() const;
		[[nodiscard]] constexpr eIODirection GetIODirection() const;
		[[nodiscard]] constexpr PinTypeID GetParentID() const;
		
		void SetName(std::string name);
		void AddSplitPinTypeID(PinTypeID pinTypeID);

	private:

		PinType(std::string name,
			GenericDataTypeID dataTypeID,
			SetPinValueF setPinValueFunction,
			SetPinValueFromPinF setPinValueFromPinFunction,
			MemoryPoolID defaultValueMemoryID,
			eIODirection ioDirection,
			PinTypeID parentID
		);

	private:

		std::string mName;
		//const DataTypeID mDataTypeID = InvalidID<DataTypeID>();
		GenericDataTypeID mGenericDataTypeID;
		SetPinValueF mSetPinValueFunction;
		SetPinValueFromPinF mSetPinValueFromPinFunction;
		MemoryPoolID mDefaultValueID = InvalidID<MemoryPoolID>();
		std::vector<PinTypeID> mSplitPinTypeIDs;
        PinTypeID mParentID = InvalidID<PinTypeID>();
		eIODirection mIODirection = eIODirection::Input;
	};


	constexpr GenericDataTypeID PinType::GetDataTypeID() const
	{
		return mGenericDataTypeID;
	}

	constexpr SetPinValueF PinType::GetSetPinValueFunction() const
	{
		return mSetPinValueFunction;
	}

	constexpr SetPinValueFromPinF PinType::GetSetPinValueFromPinFunction() const
	{
		return mSetPinValueFromPinFunction;
	}

	constexpr MemoryPoolID PinType::GetDefaultValueMemoryID() const
	{
		return mDefaultValueID;
	}

	constexpr const std::vector<PinTypeID>& PinType::GetSplitPinTypeIDs() const
	{
		return mSplitPinTypeIDs;
	}

	constexpr eIODirection PinType::GetIODirection() const
	{
		return mIODirection;
	}

	constexpr PinTypeID PinType::GetParentID() const
	{
		return mParentID;
    }
}