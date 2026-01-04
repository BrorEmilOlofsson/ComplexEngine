#pragma once
#include "../FlyDefines.hpp"
#include <vector>

namespace FLY_NAMESPACE
{

	class Pin final
	{
	public:

		Pin(PinTypeID typeID, NonOwningPtr<void> dataPtr, NodeID nodeID);

		[[nodiscard]] constexpr PinTypeID GetTypeID() const;
		void SetDataPtr(NonOwningPtr<void> dataPtr);
		[[nodiscard]] constexpr OwningPtr<void> GetDataPtr() const;
		[[nodiscard]] constexpr const std::vector<PinID>& GetConnectedPinIDs() const;
		void SetSplitPinIDs(std::vector<PinID> pinIDs);
		[[nodiscard]] constexpr const std::vector<PinID>& GetSplitPinIDs() const;
		[[nodiscard]] constexpr NodeID GetNodeID() const;
		void SetParentPinID(PinID pinID);
		[[nodiscard]] PinID GetParentPinID() const;
		void SetIsSplit(bool isSplit);
		[[nodiscard]] bool IsSplit() const;

		void AddConnectedPin(PinID pinID);
		void RemoveConnectedPin(PinID pinID);

	private:

		PinTypeID mTypeID = InvalidID<PinTypeID>();
		OwningPtr<void> mDataPtr;
		std::vector<PinID> mConnectedPinIDs;
		std::vector<PinID> mSplitPinIDs;
		NodeID mNodeID = InvalidID<NodeID>();
		PinID mParentPinID = InvalidID<PinID>();
		bool mIsSplit = false;
	};

	constexpr PinTypeID Pin::GetTypeID() const
	{
		return mTypeID;
	}

	constexpr OwningPtr<void> Pin::GetDataPtr() const
	{
		return mDataPtr;
	}

	constexpr const std::vector<PinID>& Pin::GetConnectedPinIDs() const
	{
		return mConnectedPinIDs;
	}

	constexpr const std::vector<PinID>& Pin::GetSplitPinIDs() const
	{
		return mSplitPinIDs;
	}

	constexpr NodeID Pin::GetNodeID() const
	{
		return mNodeID;
	}
}