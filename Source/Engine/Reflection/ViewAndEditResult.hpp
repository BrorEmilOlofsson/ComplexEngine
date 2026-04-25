#pragma once
#include <variant>
#include <optional>
#include "Engine/Reflection/DataTypeID.hpp"
#include "Engine/Reflection/PropertyPath.hpp"

namespace CLX
{

	namespace VectorEditOperations
	{
		struct PushBack {};
		struct Insert
		{
			std::size_t index = 0;
		};
		struct EraseElement
		{
			std::size_t index = 0;
		};
		struct Clear {};
		struct ResetElement
		{
			std::size_t index = 0;
        };
		struct ModifyElement
		{
			std::size_t index = 0;
        };
	}
	
	using VectorEditOperation = std::variant<VectorEditOperations::PushBack, VectorEditOperations::Insert, VectorEditOperations::EraseElement, VectorEditOperations::Clear, VectorEditOperations::ResetElement, VectorEditOperations::ModifyElement>;

	struct ViewAndEditResult final
	{
		bool isActive = false;
		bool isEdited = false;
        DataTypeID dataTypeID = InvalidDataTypeID;
        void* dataPtr = nullptr;
		PropertyPath propertyPath;
        std::optional<VectorEditOperation> vectorEditOperation;

        ViewAndEditResult& operator|=(const ViewAndEditResult& other)
		{
			isActive |= other.isActive;
			isEdited |= other.isEdited;
			dataTypeID = other.dataTypeID;
			dataPtr = other.dataPtr;
			propertyPath = other.propertyPath;
            vectorEditOperation = other.vectorEditOperation;
			return *this;
		}
	};
}