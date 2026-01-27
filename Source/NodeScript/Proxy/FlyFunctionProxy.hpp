#pragma once
#include "../FlyDefines.hpp"
#include "../Function/FlyFunction.hpp"
#include "FlyNodeProxy.hpp"
#include "FlyNodeTypeProxy.hpp"
#include "FlyNodeGraphProxy.hpp"
#include "FlyVariableProxy.hpp"

namespace FLY_NAMESPACE
{
	
	class FunctionProxy final
	{
	public:

		FunctionProxy() = default;
		explicit FunctionProxy(FunctionID functionID);

		[[nodiscard]] const std::string& GetName() const;
		[[nodiscard]] NodeProxy GetInputNode() const;
		[[nodiscard]] NodeProxy GetOutputNode() const;
		[[nodiscard]] NodeTypeProxy GetCallerNodeType() const;
		[[nodiscard]] NodeTypeProxy GetInputNodeType() const;
		[[nodiscard]] NodeTypeProxy GetOutputNodeType() const;
		[[nodiscard]] NodeGraphProxy GetNodeGraph();
		[[nodiscard]] std::vector<VariableProxy> GetLocalVariables() const;

		[[nodiscard]] FunctionID GetID() const;

		void SetName(std::string name, CommandTracker* commandTracker);
		void AddPin(GenericDataTypeProxy dataType, eIODirection ioDirection, std::string name, CommandTracker* commandTracker);
		void SetPinNameAtIndex(std::string name, size_t index, eIODirection ioDirection, CommandTracker* commandTracker);
		void SetPinDataTypeAtIndex(GenericDataTypeProxy dataType, size_t index, eIODirection ioDirection, CommandTracker* commandTracker);

		[[nodiscard]] explicit operator bool() const;

		[[nodiscard]] friend bool operator==(const FunctionProxy& lhs, const FunctionProxy& rhs);

	private:

		Function& GetFunction();
		const Function& GetFunction() const;

	private:

		FunctionID mFunctionID = InvalidID<FunctionID>();
	};
}