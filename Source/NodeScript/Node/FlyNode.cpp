#include "FlyNode.hpp"

namespace FLY_NAMESPACE
{
	Node::Node(const NodeTypeID typeID, const std::vector<PinID>& inputPins, const std::vector<PinID>& outputPins, const void* inputTuplePtr)
		: mTypeID(typeID)
		, mInputPins(inputPins)
		, mOutputPins(outputPins)
		, mSplitInputPins(mInputPins)
		, mSplitOutputPins(mOutputPins)
        , mInputTuplePtr(inputTuplePtr)
	{
	}

	void Node::SetPosition(const Vec2 position)
	{
		mPosition = position;
	}

	Vec2 Node::GetPosition() const
	{
		return mPosition;
	}
	
	bool Node::IsDestroyed() const
	{
		return mIsDestroyed;
	}

	void Node::SetIsDestroyed(bool isDestroyed)
	{
		mIsDestroyed = isDestroyed;
	}
}