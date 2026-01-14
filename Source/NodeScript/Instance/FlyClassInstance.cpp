#include "FlyClassInstance.hpp"
#include "../DataType/FlyClass.hpp"
#include "../Internal/FlyInternal.hpp"

namespace FLY_NAMESPACE
{

	ClassInstance::ClassInstance(ClassID classID)
		: mClassID(classID)
		, mVariableContainerInstance(Internal::GetClassByID(classID).mVariableContainer)
		, mEventGraphInstance(Internal::GetClassByID(classID).mEventGraph.GetNodeGraph())
	{
	}

	void ClassInstance::InitRuntime()
	{
		mVariableContainerInstance.InitRuntime();
	}
}
