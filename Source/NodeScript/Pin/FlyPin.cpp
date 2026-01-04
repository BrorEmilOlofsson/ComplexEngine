#include "FlyPin.hpp"

namespace FLY_NAMESPACE
{

	Pin::Pin(const PinTypeID typeID, const NonOwningPtr<void> dataPtr, const NodeID nodeID)
		: mTypeID(typeID)
		, mDataPtr(dataPtr.Get())
		, mNodeID(nodeID)
	{
	}

	void Pin::SetDataPtr(NonOwningPtr<void> dataPtr)
	{
		mDataPtr = dataPtr.Get();
	}

	void Pin::SetSplitPinIDs(std::vector<PinID> pinIDs)
	{
		mSplitPinIDs = std::move(pinIDs);
	}

	void Pin::SetParentPinID(const PinID pinID)
	{
		mParentPinID = pinID;
	}

	PinID Pin::GetParentPinID() const
	{
		return mParentPinID;
	}

	void Pin::SetIsSplit(bool isSplit)
	{
		mIsSplit = isSplit;
	}

	bool Pin::IsSplit() const
	{
		return mIsSplit;
	}

	void Pin::AddConnectedPin(const PinID pinID)
	{
		mConnectedPinIDs.push_back(pinID);
	}

	void Pin::RemoveConnectedPin(const PinID pinID)
	{
		std::erase(mConnectedPinIDs, pinID);
	}
}