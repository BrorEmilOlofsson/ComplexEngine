#include "FlyStruct.hpp"

namespace FLY_NAMESPACE
{

	Struct::Struct(std::string name)
		: mName(std::move(name))
	{
	}

	std::string& Struct::Name()
	{
		return mName;
	}

	const std::string& Struct::Name() const
	{
		return mName;
	}
}
