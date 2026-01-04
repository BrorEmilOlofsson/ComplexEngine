#include "FlyNodeType.hpp"

namespace FLY_NAMESPACE
{

	NodeType::NodeType(std::string name, CreateNodeFunction createFunction, ExecuteNodeFunction executeFunction, FastExecuteNodeFunction fastExecuteFunction,
		eNodeTrait traits, EventID eventID, DataTypeID ownerDataTypeID, std::vector<PinTypeID> inputPinTypeIDs, std::vector<PinTypeID> outputPinTypeIDs,
		MemoryPoolID functionMemoryID, DataTypeID nodeStateDataTypeID, TraitID traitID, eNodeOperatorType operatorType)
		: mName(std::move(name))
		, mCreateFunction(createFunction)
		, mExecuteFunction(executeFunction)
		, mFastExecuteFunction(fastExecuteFunction)
		, mTraits(traits)
		, mEventID(eventID)
		, mOwnerDataTypeID(ownerDataTypeID)
		, mInputPinTypeIDs(std::move(inputPinTypeIDs))
		, mOutputPinTypeIDs(std::move(outputPinTypeIDs))
		, mFunctionMemoryID(functionMemoryID)
		, mNodeStateDataTypeID(nodeStateDataTypeID)
		, mTraitID(traitID)
		, mOperatorType(operatorType)
	{
	}

	const std::string& NodeType::GetName()const
	{
		return mName;
	}

	std::vector<NodeRef>& NodeType::GetNodeRefs()
	{
		return mNodeRefs;
	}

	const std::vector<NodeRef>& NodeType::GetNodeRefs() const
	{
		return mNodeRefs;
	}

	void NodeType::SetName(std::string name)
	{
		mName = std::move(name);
	}
}