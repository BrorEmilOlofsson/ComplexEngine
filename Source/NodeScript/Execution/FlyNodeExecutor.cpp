#include "FlyNodeExecutor.hpp"
#include "../DataType/FlyClass.hpp"
#include "FlyNodeTypeManager.hpp"
#include "FlyClassInstance.hpp"
#include "../Internal/FlyInternal.hpp"
#include "FlyNodeExecutionQueue.hpp"

namespace FLY_NAMESPACE
{

	InternalExecutionContext CreateInternalExecutionContext(DataTypeManager& dataTypeManager, NodeTypeManager& nodeTypeManager, PinTypeManager& pinTypeManager, TraitManager& traitManager, MemoryPool& foundationMemoryPool, NodeExecutor& nodeExecutor)
	{
		InternalExecutionContext context
		{
			.nodeExecutor = nodeExecutor,
            .nodeTypeManager = nodeTypeManager,
            .pinTypeManager = pinTypeManager,
            .dataTypeManager = dataTypeManager,
			.traitManager = traitManager,
            .foundationMemoryPool = foundationMemoryPool
		};
		return context;
    }

	NodeExecutor::NodeExecutor(DataTypeManager& dataTypeManager, NodeTypeManager& nodeTypeManager, PinTypeManager& pinTypeManager, TraitManager& traitManager, MemoryPool& foundationMemoryPool)
        : mExecutionContext(CreateInternalExecutionContext(dataTypeManager, nodeTypeManager, pinTypeManager, traitManager, foundationMemoryPool, *this))
	{
	}

	void NodeExecutor::ExecuteNode(const NodeExecutionData& nodeExecutionData)
	{
		const Node& node = nodeExecutionData.nodeRef.GetNodeGraph().GetNode(nodeExecutionData.nodeRef.GetNodeID());
		const NodeType& nodeType = mExecutionContext.nodeTypeManager.GetNodeType(node.GetTypeID());
		nodeType.GetExecuteFunction().Invoke(nodeExecutionData, mExecutionContext);
	}

	void NodeExecutor::ExecuteEvent(const EventID eventID, ClassInstance& classInstance, void* const target, const ExecutionContextBase& executionContext)
	{
		Class& c = Internal::GetClassByID(classInstance.mClassID);
		mExecutionContext.flyClass = &c;
		mExecutionContext.executionContext = &executionContext;
		mExecutionContext.classInstance = &classInstance;
		mExecutionContext.nodeGraphInstance = &classInstance.mEventGraphInstance;
		mExecutionContext.target = target;
		mExecutionContext.nodeGraphVariantHandle = &c.mEventGraph;



#ifdef FLY_DEBUG
		if (target == nullptr)
		{
			assert(c.mTargetID == GetDataTypeID<None*>());
		}
#endif

		EventGraph& eventGraph = c.mEventGraph;
		auto nodeIDs = eventGraph.GetNodeIDsByEventID(eventID);


		if (!nodeIDs)
		{
			return;
		}


		NodeExecutionQueue nodeExecutionQueue(*this);

		mExecutionContext.nodeExecutionQueue = &nodeExecutionQueue;

		for (const NodeID nodeID : nodeIDs.value())
		{
			nodeExecutionQueue.Push(NodeExecutionData{ .nodeRef = NodeRef{ nodeID, eventGraph.GetNodeGraph() },
				.triggerReason = eNodeTriggerReason::Event });
		}

		nodeExecutionQueue.Execute();
	}

	Debugger& NodeExecutor::GetDebugger()
	{
		return mDebugger;
	}

}

