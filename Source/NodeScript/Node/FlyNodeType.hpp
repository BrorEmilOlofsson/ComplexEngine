#pragma once
#include <vector>
#include <string>
#include <any>

#include "../FlyDefines.hpp"
#include "FlyNodeTrait.hpp"
#include "FlyNodeRef.hpp"
#include "../Utilities/FlyFunctional.hpp"

namespace FLY_NAMESPACE
{

	class Node;
	class NodeGraph;
	struct InternalExecutionContext;
	struct NodeExecutionData;
	class MemoryPool;
	class NodeType;
    struct NodeCreationContext;

	using CreateNodeFunction = FunctionWrapper<Node(const NodeID nodeID, const NodeTypeID nodeTypeID, NodeGraph& nodeGraph, const NodeCreationContext& creationContext), struct CreateNodeParam>;
	using ExecuteNodeFunction = FunctionWrapper<void(const NodeExecutionData& nodeExecutionData, InternalExecutionContext& context), struct ExecuteNodeParam>;
	using FastExecuteNodeFunction = FunctionWrapper<void(InternalExecutionContext& context, const MemoryPool& foundationMemoryPool, const NodeType& nodeType, const void* mainInput, const void* inputTuple, void* outputValue), struct FastExecuteNodeParam>;


	class NodeType final
	{
	public:

		NodeType(std::string name, CreateNodeFunction createFunction, ExecuteNodeFunction executeFunction, FastExecuteNodeFunction fastExecuteFunction,
			eNodeTrait traits, EventID eventID, DataTypeID ownerDataTypeID, std::vector<PinTypeID> inputPinTypeIDs, std::vector<PinTypeID> outputPinTypeIDs,
			MemoryPoolID functionMemoryID, DataTypeID nodeStateDataTypeID, TraitID traitID, eNodeOperatorType nodeOperatorType);

		[[nodiscard]] const std::string& GetName() const;
		[[nodiscard]] constexpr CreateNodeFunction GetCreateFunction() const;
		[[nodiscard]] constexpr ExecuteNodeFunction GetExecuteFunction() const;
		[[nodiscard]] constexpr FastExecuteNodeFunction GetFastExecuteFunction() const;
		[[nodiscard]] constexpr eNodeTrait GetTraits() const;
		[[nodiscard]] constexpr EventID GetEventID() const;
		[[nodiscard]] constexpr DataTypeID GetOwnerDataTypeID() const;
		[[nodiscard]] constexpr std::vector<PinTypeID>& GetInputPinTypeIDs();
		[[nodiscard]] constexpr const std::vector<PinTypeID>& GetInputPinTypeIDs() const;
		[[nodiscard]] constexpr std::vector<PinTypeID>& GetOutputPinTypeIDs();
		[[nodiscard]] constexpr const std::vector<PinTypeID>& GetOutputPinTypeIDs() const;
		[[nodiscard]] constexpr MemoryPoolID GetFunctionMemoryID() const;
		[[nodiscard]] constexpr DataTypeID GetNodeStateDataTypeID() const;
		[[nodiscard]] constexpr TraitID GetTraitID() const;
		[[nodiscard]] constexpr eNodeOperatorType GetOperatorType() const;

		void SetName(std::string name);

	private:

		std::string mName;
		CreateNodeFunction mCreateFunction;
		ExecuteNodeFunction mExecuteFunction;
		FastExecuteNodeFunction mFastExecuteFunction;
		eNodeTrait mTraits = eNodeTrait::None;
		EventID mEventID = InvalidID<EventID>();
		DataTypeID mOwnerDataTypeID;
		std::vector<PinTypeID> mInputPinTypeIDs;
		std::vector<PinTypeID> mOutputPinTypeIDs;
		MemoryPoolID mFunctionMemoryID = InvalidID<MemoryPoolID>();
		DataTypeID mNodeStateDataTypeID;
		TraitID mTraitID;
		eNodeOperatorType mOperatorType = eNodeOperatorType::None;
	};

	constexpr CreateNodeFunction NodeType::GetCreateFunction() const
	{
		return mCreateFunction;
	}

	constexpr ExecuteNodeFunction NodeType::GetExecuteFunction() const
	{
		return mExecuteFunction;
	}

	constexpr FastExecuteNodeFunction NodeType::GetFastExecuteFunction() const
	{
		return mFastExecuteFunction;
	}

	constexpr eNodeTrait NodeType::GetTraits() const
	{
		return mTraits;
	}

	constexpr EventID NodeType::GetEventID() const
	{
		return mEventID;
	}

	constexpr DataTypeID NodeType::GetOwnerDataTypeID() const
	{
		return mOwnerDataTypeID;
	}

	constexpr std::vector<PinTypeID>& NodeType::GetInputPinTypeIDs()
	{
		return mInputPinTypeIDs;
	}

	constexpr const std::vector<PinTypeID>& NodeType::GetInputPinTypeIDs() const
	{
		return mInputPinTypeIDs;
	}

	constexpr std::vector<PinTypeID>& NodeType::GetOutputPinTypeIDs()
	{
		return mOutputPinTypeIDs;
	}

	constexpr const std::vector<PinTypeID>& NodeType::GetOutputPinTypeIDs() const
	{
		return mOutputPinTypeIDs;
	}

	constexpr MemoryPoolID NodeType::GetFunctionMemoryID() const
	{
		return mFunctionMemoryID;
	}

	constexpr DataTypeID NodeType::GetNodeStateDataTypeID() const
	{
		return mNodeStateDataTypeID;
	}

	constexpr TraitID NodeType::GetTraitID() const
	{
		return mTraitID;
	}

	constexpr eNodeOperatorType NodeType::GetOperatorType() const
	{
		return mOperatorType;
	}

	struct NodeTypeDesc final
	{
		std::vector<std::string> mInputPinNames;
		std::vector<std::string> mOutputPinNames;
		std::vector<std::any> mDefaultValues;
		bool mShowDataTypePinNames = true;
	};
}