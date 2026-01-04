#pragma once
#include "../FlyDefines.hpp"
#include "FlyVariableProxy.hpp"

namespace FLY_NAMESPACE
{

	class Struct;
	class DataTypeProxy;
	class GenericDataTypeProxy;

	class StructProxy final
	{
	public:
		StructProxy() = default;
		StructProxy(StructID structID);


		[[nodiscard]] std::string_view GetName() const;
		[[nodiscard]] StructID GetID() const;
		[[nodiscard]] std::vector<VariableProxy> GetMemberVariables(bool includeDestroyed = false) const;

		VariableProxy CreateVariable(const GenericDataTypeProxy& dataTypeProxy, std::string name, CommandTracker* const commandTracker);
		void SetName(std::string name, CommandTracker* commandTracker);

		explicit operator bool() const;

		friend bool operator==(const StructProxy& lhs, const StructProxy& rhs);

	private:


		[[nodiscard]] Struct& GetStruct() const;

	private:

		StructID mStructID = InvalidID<StructID>();
	};
}