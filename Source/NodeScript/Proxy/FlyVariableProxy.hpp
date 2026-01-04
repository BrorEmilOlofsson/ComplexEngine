#pragma once
#include "../FlyDefines.hpp"

namespace FLY_NAMESPACE
{
	class DataTypeProxy;
	class StructProxy;
	class ClassProxy;
	class GenericDataTypeProxy;
	class Variable;
	class CommandTracker;

	class VariableProxy final
	{
	public:

		VariableProxy() = default;
		VariableProxy(VarID varID, const ClassProxy& flyClass);
		VariableProxy(VarID varID, const StructProxy& flyStruct);
		VariableProxy(VarID varID, const DataTypeProxy& dataType);
		VariableProxy(VarID varID, const GenericDataTypeProxy& genericDataType);

		[[nodiscard]] std::string_view GetName() const;
		[[nodiscard]] GenericDataTypeID GetDataTypeID() const;
		[[nodiscard]] bool IsDestroyed() const;
		[[nodiscard]] VarID GetID() const;
		[[nodiscard]] size_t GetByteOffset() const;

		void SetName(std::string name, CommandTracker* commandTracker);
		void Destroy(CommandTracker* commandTracker);
		void ViewAndEditDefaultValue(CommandTracker* commandTracker);
		void SetDataType(GenericDataTypeProxy dataTypeProxy, CommandTracker* commandTracker);

		explicit operator bool() const;

	private:

		const Variable& GetVariable() const;
	private:

		VarID mVarID = InvalidID<VarID>();
		GenericDataTypeID mOwnerID;
	};
}