#pragma once
#include <vector>
#include <string>

#include "../FlyDefines.hpp"
#include "FlyVariableContainer.hpp"
#include "../Graph/FlyEventGraph.hpp"
#include "../Memory/FlyHeapObject.hpp"

namespace FLY_NAMESPACE
{

	class Function;
	class ClassInstance;

	class Class final
	{
	public:

		Class(DataTypeID targetID = GetDataTypeID<None>(), std::string name = "Default Class");
		~Class() = default;

		Class(const Class&) = delete;
		Class(Class&&) = delete;
		Class& operator=(const Class&) = delete;
		Class& operator=(Class&&) = delete;

	public:

		EventGraph mEventGraph;
		VariableContainer mVariableContainer;

		std::vector<HeapObject<ClassInstance>> mClassInstances;

		DataTypeID mTargetID;
		std::string mName;
	};
}

