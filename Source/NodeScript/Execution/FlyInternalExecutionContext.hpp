#pragma once
#include "../FlyDefines.hpp"
#include "../Node/FlyNodeRef.hpp"
#include "../Graph/FlyNodeGraphVariant.hpp"
#include <queue>

namespace FLY_NAMESPACE
{

	enum class eNodeTriggerReason : uint8_t
	{
		Flow,
		Event,
		Read
	};

	struct NodeExecutionData final
	{
		NodeRef nodeRef;
		eNodeTriggerReason triggerReason = eNodeTriggerReason::Flow;
	};

	constexpr bool operator==(const NodeExecutionData& lhs, const NodeExecutionData& rhs)
	{
		return lhs.nodeRef == rhs.nodeRef;
	}

	constexpr bool operator<(const NodeExecutionData& lhs, const NodeExecutionData& rhs)
	{
		return lhs.nodeRef < rhs.nodeRef;
	}

	class Class;
	class InternalModifier;
	struct ExecutionContextBase;
	class ClassInstance;
	class NodeGraphInstance;
	class NodeExecutor;
	class NodeExecutionQueue;
	class NodeTypeManager;
	class PinTypeManager;
	class DataTypeManager;
	class TraitManager;
	class MemoryPool;

	struct InternalExecutionContext final
	{
		Class* flyClass = nullptr;
		const ExecutionContextBase* executionContext;
		NodeExecutionData nodeData;
		NodeGraphVariantHandle nodeGraphVariantHandle;
		NodeExecutionQueue* nodeExecutionQueue = nullptr;
		void* target = nullptr;
		ClassInstance* classInstance = nullptr;
		NodeGraphInstance* nodeGraphInstance = nullptr;
		NodeExecutor& nodeExecutor;
		NodeTypeManager& nodeTypeManager;
		PinTypeManager& pinTypeManager;
		DataTypeManager& dataTypeManager;
		TraitManager& traitManager;
		MemoryPool& foundationMemoryPool;
	};
}

template<>
struct std::hash<FLY_NAMESPACE::NodeExecutionData>
{
	std::size_t operator()(const FLY_NAMESPACE::NodeExecutionData& value) const
	{
		return static_cast<std::size_t>(value.nodeRef.GetNodeID());
	}
};