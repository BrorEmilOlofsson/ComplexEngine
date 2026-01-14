#include "FlyVariableContainer.hpp"


namespace FLY_NAMESPACE
{

	VariableContainer::VariableContainer(const VariableContainer& other)
		: mVariables(other.mVariables)
		, mMemoryArena(other.mMemoryArena)
	{
		for (size_t i = 0; i < mVariables.size(); i++)
		{
			mVariables[i].SetDefaultValueDataPtr(mMemoryArena.GetRenewedPointer(other.mVariables[i].GetDefaultValueDataPtr(), other.mMemoryArena));
		}
	}

	VariableContainer& VariableContainer::operator=(const VariableContainer& other)
	{
		mVariables = other.mVariables;
		mMemoryArena = other.mMemoryArena;

		for (size_t i = 0; i < mVariables.size(); i++)
		{
			mVariables[i].SetDefaultValueDataPtr(mMemoryArena.GetRenewedPointer(other.mVariables[i].GetDefaultValueDataPtr(), other.mMemoryArena));
		}

		return *this;
	}

	void VariableContainer::AddVariable(Variable variable)
	{
		mVariables.push_back(std::move(variable));
	}
}