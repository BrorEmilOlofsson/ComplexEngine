#pragma once
#include "Engine/Reflection/DataTypeID.hpp"
#include "Engine/Reflection/PropertyPath.hpp"

namespace CLX
{
	struct ViewAndEditResult final
	{
		bool isActive = false;
		bool isEdited = false;
        DataTypeID dataTypeID = InvalidDataTypeID;
        void* dataPtr = nullptr;
		PropertyPath propertyPath;

		friend constexpr ViewAndEditResult& operator|=(ViewAndEditResult& a, const ViewAndEditResult other) noexcept
		{
			a.isActive |= other.isActive;
			a.isEdited |= other.isEdited;
			a.dataTypeID = other.dataTypeID;
			a.dataPtr = other.dataPtr;
			return a;
		}
	};
}