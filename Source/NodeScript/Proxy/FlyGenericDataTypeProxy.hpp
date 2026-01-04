#pragma once
#include <string>
#include "../FlyDefines.hpp"
#include "FlyVariableProxy.hpp"
#include "../DataType/FlyGenericDataType.hpp"
#include "../DataType/FlyDataTypeTrait.hpp"

namespace FLY_NAMESPACE
{

	class DataTypeProxy;
	class DataType;

	class GenericDataTypeProxy final
	{
	public:

		GenericDataTypeProxy() = default;
		explicit GenericDataTypeProxy(GenericDataTypeID dataTypeID);
		explicit GenericDataTypeProxy(const DataTypeProxy& dataType);
		explicit GenericDataTypeProxy(DataTypeID dataTypeID);
		explicit GenericDataTypeProxy(ClassID classID);

		[[nodiscard]] GenericDataTypeID GetID() const;

		[[nodiscard]] const std::string& GetName() const;
		[[nodiscard]] Color GetColor() const;
		[[nodiscard]] eDataTypeTrait GetTypeTraits() const;
		[[nodiscard]] size_t GetSize() const;
		[[nodiscard]] size_t GetAlignment() const;

		[[nodiscard]] bool IsTargetable() const;
		[[nodiscard]] bool IsPointer() const;
		[[nodiscard]] bool IsViewAndEditable() const;
		[[nodiscard]] bool IsViewable() const;
		[[nodiscard]] std::vector<VariableProxy> GetMemberVariables() const;

		void SetColor(const Color& color);
		VariableProxy CreateMemberVariable(GenericDataTypeProxy dataTypeProxy, std::string name, CommandTracker* const commandTracker);

		[[nodiscard]] explicit operator bool() const;

		friend bool operator==(const GenericDataTypeProxy& lhs, const GenericDataTypeProxy& rhs);

	private:

		[[nodiscard]] DataType* GetDataType() const;

	private:

		GenericDataTypeID mDataTypeID;
	};

}