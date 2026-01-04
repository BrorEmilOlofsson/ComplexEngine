#pragma once
#include <filesystem>
#include <string>

#include "../FlyDefines.hpp"
#include "FlyVariableProxy.hpp"
#include "FlyFunctionProxy.hpp"
#include "FlyDataTypeProxy.hpp"
#include "FlyNodeGraphProxy.hpp"
#include "FlyClassInstanceProxy.hpp"

namespace FLY_NAMESPACE
{

	class Class;
	class EventGraph;

	class ClassProxy final
	{
	public:

		ClassProxy() = default;
		explicit ClassProxy(ClassID classID);
		
		[[nodiscard]] std::string_view GetName() const;
		[[nodiscard]] DataTypeProxy GetTargetDataType() const;

		[[nodiscard]] NodeGraphProxy GetEventGraph() const;

		[[nodiscard]] std::vector<VariableProxy> GetVariables(bool includeDestroyed = false) const;
		[[nodiscard]] std::vector<FunctionProxy> GetFunctions() const;

		VariableProxy CreateVariable(GenericDataTypeProxy dataType, std::string name, CommandTracker* commandTracker);
		ClassInstanceProxy CreateClassInstance();
		FunctionProxy CreateMemberFunction(std::string name);

		[[nodiscard]] ClassID GetID() const;

		void SetName(std::string name, CommandTracker* commandTracker);

		void Save(const std::filesystem::path& savePath) const;

		[[nodiscard]] explicit operator bool() const;

		friend bool operator==(const ClassProxy& lhs, const ClassProxy& rhs);

	private:

		[[nodiscard]] Class& GetClass() const;

	private:

		ClassID mClassID = InvalidID<ClassID>();
	};
}