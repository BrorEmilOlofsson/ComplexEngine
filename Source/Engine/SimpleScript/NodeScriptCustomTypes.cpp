#include "Engine/Precompiled/EnginePch.hpp"
#include "NodeScriptCustomTypes.hpp"
#include "NodeScript/FlyRegistration.hpp"


Fly::ViewAndEditResult ViewAndEditDataPtr(Fly::DataTypeID dataTypeID, void* dataPtr)
{
	return Fly::Internal::GetDataTypeManager().ViewAndEditData(dataTypeID, dataPtr);
}

void SaveDataPtr(Fly::DataTypeID dataTypeID, const void* dataPtr, nlohmann::json& json)
{
	Fly::Internal::GetDataTypeManager().SaveData(dataTypeID, dataPtr, json);
}

void LoadDataPtr(Fly::DataTypeID dataTypeID, void* dataPtr, const nlohmann::json& json)
{
	Fly::Internal::GetDataTypeManager().LoadData(dataTypeID, dataPtr, json);
}
