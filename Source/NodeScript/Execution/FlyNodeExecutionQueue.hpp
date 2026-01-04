#pragma once
#include <vector>
#include "../FlyDefines.hpp"
#include "FlyInternalExecutionContext.hpp"

namespace FLY_NAMESPACE
{

    class NodeExecutor;

    class NodeExecutionQueue final
    {
    public:

        NodeExecutionQueue(NodeExecutor& nodeExecutor);

        constexpr void Push(const NodeExecutionData& node);

        void Execute();

        constexpr void Clear();

    private:

        std::vector<NodeExecutionData> mExecutionQueue;

        size_t mCurrentInsertionIndex = 0;

        NodeExecutor* mNodeExecutor = nullptr;

    };

    constexpr void NodeExecutionQueue::Push(const NodeExecutionData& node)
    {
        mExecutionQueue.insert(mExecutionQueue.begin() + mCurrentInsertionIndex, node);
        ++mCurrentInsertionIndex;
    }

    constexpr void NodeExecutionQueue::Clear()
    {
        mExecutionQueue.clear();
    }
}
