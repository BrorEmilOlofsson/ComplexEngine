#pragma once
#include "../FlyDefines.hpp"
#include "../Node/FlyNodeRef.hpp"
#include <stack>

namespace FLY_NAMESPACE
{

	class NodeGraph;

	class CallStack final
	{
	public:


		CallStack() = default;
		
		void Push(const NodeRef& nodeRef);
		NodeRef Pop();

	private:

		std::stack<NodeRef> mCallStack;

	};
}

