#include "FlyNodeExecutionQueue.hpp"
#include "FlyNodeExecutor.hpp"

namespace FLY_NAMESPACE
{
	NodeExecutionQueue::NodeExecutionQueue(NodeExecutor& nodeExecutor)
		: mNodeExecutor(&nodeExecutor)
	{
	}

	void NodeExecutionQueue::Execute()
	{
		mExecutionQueue.shrink_to_fit();
		for (size_t i = 0; i < mExecutionQueue.size(); i++)
		{
			mCurrentInsertionIndex = i + 1;
			mNodeExecutor->ExecuteNode(mExecutionQueue[i]);
		}
	}
}
