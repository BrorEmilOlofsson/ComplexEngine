#include "FlyClass.hpp"
#include "Instance/FlyClassInstance.hpp"

namespace FLY_NAMESPACE
{

	Class::Class(const DataTypeID targetID, const std::string& name)
		: mTargetID(targetID)
		, mName(name)
	{
	}

	Class::~Class()
	{
	}

	void Class::BindFunction(FunctionID functionID)
	{
		mMemberFunctionIDs.push_back(functionID);
	}

	const std::vector<FunctionID>& Class::GetMemberFunctionIDs() const
	{
		return mMemberFunctionIDs;
	}
}

