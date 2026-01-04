#include "FlyCallStack.hpp"
#include "FlyNodeGraph.hpp"

namespace FLY_NAMESPACE
{

	void CallStack::Push(const NodeRef& nodeRef)
	{
		mCallStack.push(nodeRef);
	}


	NodeRef CallStack::Pop()
	{
		NodeRef top = mCallStack.top();
		mCallStack.pop();
		return top;
	}

}

