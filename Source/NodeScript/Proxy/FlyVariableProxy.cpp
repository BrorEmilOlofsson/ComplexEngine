#include "FlyVariableProxy.hpp"
#include "../DataType/FlyDataType.hpp"
#include "../DataType/FlyClass.hpp"
#include "../DataType/FlyStruct.hpp"
#include "../Fly.hpp"
#include "../Internal/FlyInternal.hpp"
#include "../DataType/FlyDataTypeManager.hpp"
#include "../Node/FlyNodeTypeManager.hpp"

namespace FLY_NAMESPACE
{

	VariableProxy::VariableProxy(const VarID varID, const ClassProxy& flyClass)
		: mVarID(varID)
		, mOwnerID(GenericDataTypeID{ flyClass.GetID() })
	{
	}

	VariableProxy::VariableProxy(const VarID varID, const DataTypeProxy& dataType)
		: mVarID(varID)
		, mOwnerID(GenericDataTypeID{ dataType.GetID() })
	{
	}

	VariableProxy::VariableProxy(VarID varID, const GenericDataTypeProxy& genericDataType)
		: mVarID(varID)
		, mOwnerID(genericDataType.GetID())
	{
	}

	VariableProxy::VariableProxy(const VarID varID, const FunctionID functionID)
		: mVarID(varID)
		, mOwnerID(functionID)
	{
    }


	// Temp
	static VariableContainer* GetVariableContainerInternal(std::variant<GenericDataTypeID, FunctionID> owner)
	{
		return std::visit(Visitor
			{
				[](const GenericDataTypeID dataTypeID) -> VariableContainer*
				{
					return std::visit(Visitor
					{
						[&](const DataTypeID dataTypeID) -> VariableContainer* { return &Internal::GetDataTypeManager().Find(dataTypeID)->GetVariableContainer(); },
						[&](const ClassID classID) -> VariableContainer* { return &Internal::GetClassByID(classID).mVariableContainer; }
					}, dataTypeID.mID);
				},
				[](const FunctionID functionID) -> VariableContainer*
				{
					return &Internal::GetNodeTypeManager().GetFunction(functionID).GetVariableContainer();
                }
			}, owner);
	}

	std::string_view VariableProxy::GetName() const
	{
		return GetVariableContainerInternal(mOwnerID)->GetVariable(mVarID).Name();
	}

	GenericDataTypeID VariableProxy::GetDataTypeID() const
	{
		return GetVariable().GetDataTypeID();
	}

	bool VariableProxy::IsDestroyed() const
	{
		return GetVariable().IsDestroyed();
	}

	VarID VariableProxy::GetID() const
	{
		return mVarID;
	}

	size_t VariableProxy::GetByteOffset() const
	{
		return GetVariable().GetByteOffset();
	}

	void VariableProxy::SetName(std::string name, CommandTracker* const commandTracker)
	{
		Internal::SetVariableName(mVarID, *GetVariableContainerInternal(mOwnerID), std::move(name), commandTracker);
	}

	void VariableProxy::Destroy(CommandTracker* const commandTracker)
	{
		Internal::DestroyVariable(mVarID, *GetVariableContainerInternal(mOwnerID), commandTracker);
	}

	void VariableProxy::ViewAndEditDefaultValue(CommandTracker* const commandTracker)
	{
		Internal::ViewAndEditVariableDefaultValue(mVarID, *GetVariableContainerInternal(mOwnerID), commandTracker);
	}

	void VariableProxy::SetDataType(const GenericDataTypeProxy dataTypeProxy, CommandTracker* const commandTracker)
	{
		Internal::SetVariableDataType(mVarID, *GetVariableContainerInternal(mOwnerID), dataTypeProxy.GetID(), commandTracker);
	}

	VariableProxy::operator bool() const
	{
		return mVarID != InvalidID<VarID>();
	}

	const Variable& VariableProxy::GetVariable() const
	{
		return GetVariableContainerInternal(mOwnerID)->GetVariable(mVarID);
	}
}
