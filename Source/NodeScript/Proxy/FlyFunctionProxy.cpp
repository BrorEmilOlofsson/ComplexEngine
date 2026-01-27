#include "FlyFunctionProxy.hpp"
#include "../Node/FlyNodeTypeManager.hpp"
#include "../Internal/FlyInternal.hpp"

namespace FLY_NAMESPACE
{

	FunctionProxy::FunctionProxy(const FunctionID aFunctionID)
		: mFunctionID(aFunctionID)
	{
	}

	const std::string& FunctionProxy::GetName() const
	{
		return GetFunction().GetName();
	}

	NodeProxy FunctionProxy::GetInputNode() const
	{
		return NodeProxy(Function::INPUT_NODE_ID, NodeGraphProxy(*this));
	}

	NodeProxy FunctionProxy::GetOutputNode() const
	{
		return NodeProxy(Function::OUTPUT_NODE_ID, NodeGraphProxy(*this));
	}

	NodeTypeProxy FunctionProxy::GetCallerNodeType() const
	{
		const Function& function = GetFunction();
		return NodeTypeProxy(function.GetCallerNodeTypeID());
	}

	NodeTypeProxy FunctionProxy::GetInputNodeType() const
	{
		const Function& function = GetFunction();
		return NodeTypeProxy(function.GetInputNodeTypeID());
	}

	NodeTypeProxy FunctionProxy::GetOutputNodeType() const
	{
		const Function& function = GetFunction();
		return NodeTypeProxy(function.GetOutputNodeTypeID());
	}

	NodeGraphProxy FunctionProxy::GetNodeGraph()
	{
		return NodeGraphProxy(*this);
	}

	std::vector<VariableProxy> FunctionProxy::GetLocalVariables() const
	{
		const Function& function = GetFunction();
		const auto& localVariables = function.GetVariableContainer().GetVariables();
		std::vector<VariableProxy> localVariableProxies;
		localVariableProxies.reserve(localVariables.size());
		for (VarID varID = VarID{ 0 }; varID < localVariables.size(); ++varID)
		{
            localVariableProxies.emplace_back(Fly::VariableProxy(varID, mFunctionID));
		}
		return localVariableProxies;
    }

	FunctionID FunctionProxy::GetID() const
	{
		return mFunctionID;
	}

	void FunctionProxy::SetName(std::string name, [[maybe_unused]] CommandTracker* const commandTracker)
	{
		Function& function = Internal::GetNodeTypeManager().GetFunction(mFunctionID);

		function.SetName(std::move(name));
	}

	void FunctionProxy::AddPin(GenericDataTypeProxy aDataTypeProxy, eIODirection aIODirection, std::string name, CommandTracker* commandTracker)
	{
		Internal::AddPinTypeToFunction(mFunctionID, aDataTypeProxy.GetID(), aIODirection, std::move(name), commandTracker);
	}

	void FunctionProxy::SetPinNameAtIndex(std::string name, size_t index, eIODirection ioDirection, CommandTracker* commandTracker)
	{
		Internal::SetPinNameAtIndexFunction(mFunctionID, std::move(name), index, ioDirection, commandTracker);
	}

	void FunctionProxy::SetPinDataTypeAtIndex(GenericDataTypeProxy aDataTypeProxy, size_t aIndex, eIODirection ioDirection, CommandTracker* commandTracker)
	{
		Internal::SetPinDataTypeAtIndexFunction(mFunctionID, aDataTypeProxy.GetID(), aIndex, ioDirection, commandTracker);
	}

	FunctionProxy::operator bool() const
	{
		return mFunctionID != InvalidID<FunctionID>();
	}

	bool operator==(const FunctionProxy& lhs, const FunctionProxy& rhs)
	{
		return lhs.mFunctionID == rhs.mFunctionID;
	}

	Function& FunctionProxy::GetFunction()
	{
		return Internal::GetNodeTypeManager().GetFunction(mFunctionID);
	}

	const Function& FunctionProxy::GetFunction() const
	{
		return Internal::GetNodeTypeManager().GetFunction(mFunctionID);
	}
}
