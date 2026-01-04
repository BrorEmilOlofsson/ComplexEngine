#include "FlyStructProxy.hpp"
#include "../DataType/FlyStruct.hpp"
#include "../Internal/FlyInternal.hpp"
#include "FlyDataTypeProxy.hpp"
#include "FlyGenericDataTypeProxy.hpp"

namespace FLY_NAMESPACE
{

	

	StructProxy::StructProxy(const StructID structID)
		: mStructID(structID)
	{
	}

	std::string_view StructProxy::GetName() const
	{
		return GetStruct().Name();
	}

	StructID StructProxy::GetID() const
	{
		return mStructID;
	}

	std::vector<VariableProxy> StructProxy::GetMemberVariables(const bool includeDestroyed) const
	{
		std::vector<VariableProxy> variableProxys;

		Struct& s = GetStruct();

		variableProxys.reserve(s.GetVariableContainer().GetVariables().size());
		for (size_t i = 0; i < s.GetVariableContainer().GetVariableCount(); i++)
		{
			if (!includeDestroyed && s.GetVariableContainer().GetVariable(i).IsDestroyed())
			{
				continue;
			}
			variableProxys.push_back(VariableProxy(VarID{ i }, *this));
		}

		return variableProxys;

	}

	VariableProxy StructProxy::CreateVariable(const GenericDataTypeProxy& dataTypeProxy, std::string name, CommandTracker* const commandTracker)
	{
		VarID varID = Internal::CreateVariable(GetStruct().GetVariableContainer(), dataTypeProxy.GetID(), std::move(name), commandTracker);
		return VariableProxy(varID, *this);
	}

	void StructProxy::SetName(std::string name, CommandTracker* const commandTracker)
	{
		name;
		commandTracker;
		//Internal::SetStructName(mStructID, aName, aCommandTracker);
	}

	Struct& StructProxy::GetStruct() const
	{
		static Struct s;
		return s;
	}

	StructProxy::operator bool() const
	{
		return mStructID != InvalidID<StructID>();
	}

	bool operator==(const StructProxy& lhs, const StructProxy& rhs)
	{
		return lhs.mStructID == rhs.mStructID;
	}
}
