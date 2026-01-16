#include "FlyClass.hpp"
#include "Instance/FlyClassInstance.hpp"

namespace FLY_NAMESPACE
{

	Class::Class(const DataTypeID targetID, std::string name)
		: mTargetID(targetID)
		, mName(std::move(name))
	{
	}
}

