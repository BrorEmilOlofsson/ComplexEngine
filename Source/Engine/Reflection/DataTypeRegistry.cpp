#include "Engine/Precompiled/EnginePch.hpp"
#include "Engine/Reflection/DataTypeRegistry.hpp"
#include <External/imgui/imgui.h>

namespace CLX
{

	void DataTypeRegistry::Assert() const
	{
		for (auto& [_, dataType] : mDataTypes)
		{
			const bool hasEditFunction = dataType.viewAndEdit;
			const bool hasSaveFunction = dataType.toJSON;
			const bool hasLoadFunction = dataType.fromJSON;

			const uint8_t n = static_cast<uint8_t>(hasEditFunction) +
				static_cast<uint8_t>(hasSaveFunction) +
				static_cast<uint8_t>(hasLoadFunction);

			if (n != 0 && n != 3)
			{
				ASSERT(false && "DataType must have either all or none of the edit, save, and load functions defined.");
			}
		}
	}

	void DataTypeRegistry::InplaceAllocateData(DataTypeID dataTypeID, void* dataPtr, const void* defaultValuePtr) const
	{
		mDataTypes.at(dataTypeID).inplaceAllocate(dataPtr, defaultValuePtr);
	}

	void DataTypeRegistry::CopyData(DataTypeID dataTypeID, void* destination, const void* source) const
	{
		mDataTypes.at(dataTypeID).copy(destination, source);
	}

	void DataTypeRegistry::SwapData(DataTypeID dataTypeID, void* dataPtr1, void* dataPtr2) const
	{
		mDataTypes.at(dataTypeID).swap(dataPtr1, dataPtr2);
	}

	void DataTypeRegistry::DestroyData(DataTypeID dataTypeID, void* dataPtr) const
	{
		mDataTypes.at(dataTypeID).destroy(dataPtr);
	}

	size_t DataTypeRegistry::GetDataTypeSize(DataTypeID dataTypeID) const
	{
		return mDataTypes.at(dataTypeID).size;
	}

	InPlaceAllocateFunction DataTypeRegistry::GetInplaceAllocateFunction(DataTypeID dataTypeID) const
	{
		return mDataTypes.at(dataTypeID).inplaceAllocate;
	}

	DestroyFunction DataTypeRegistry::GetDestroyFunction(DataTypeID dataTypeID) const
	{
		return mDataTypes.at(dataTypeID).destroy;
	}

	CopyFunction DataTypeRegistry::GetCopyFunction(DataTypeID dataTypeID) const
	{
		return mDataTypes.at(dataTypeID).copy;
	}

	DataType* DataTypeRegistry::Find(DataTypeID dataTypeID)
	{
		auto it = mDataTypes.find(dataTypeID);
		if (it == mDataTypes.end())
		{
			return nullptr;
		}
		return &it->second;
	}

	const DataType* DataTypeRegistry::Find(DataTypeID dataTypeID) const
	{
		auto it = mDataTypes.find(dataTypeID);
		if (it == mDataTypes.end())
		{
			return nullptr;
		}
		return &it->second;
	}

	DataTypeID DataTypeRegistry::Find(std::string_view name) const
	{
		auto it = mNameToID.find(std::string(name));
		if (it == mNameToID.end())
		{
			return InvalidDataTypeID;
		}

		return it->second;
	}

	static void* PtrAdd(void* ptr, const std::size_t offset)
	{
		return reinterpret_cast<char*>(ptr) + offset;
	}

	static const void* PtrAdd(const void* ptr, const std::size_t offset)
	{
		return reinterpret_cast<const char*>(ptr) + offset;
	}

	ViewAndEditResult DataTypeRegistry::ViewAndEditData(DataTypeID dataTypeID, void* dataPtr, const Blackboard& blackboard) const
	{
		const DataType* dataType = Find(dataTypeID);
		if (dataType == nullptr)
		{
			return ViewAndEditResult{};
		}
		if (dataType->viewAndEdit != nullptr)
		{
			return dataType->viewAndEdit(dataPtr, blackboard);
		}

		ViewAndEditResult viewAndEditResult;
		for (auto& member : dataType->memberVariables)
		{
			if (!member.shouldExpose)
			{
				continue;
			}

			if (!member.canEdit)
			{
				ImGui::BeginDisabled();
			}

			void* memberDataPtr = PtrAdd(dataPtr, member.byteOffset);
			Blackboard newBlackboard = blackboard;
			newBlackboard.Insert<Key_VariableName>(member.customVariableName);
			viewAndEditResult |= ViewAndEditData(member.dataTypeID, memberDataPtr, newBlackboard);

			if (!member.canEdit)
			{
				ImGui::EndDisabled();
			}
		}
		return viewAndEditResult;
	}

	void DataTypeRegistry::LoadDataJSON(const DataType& dataType, void* dataPtr, const nlohmann::json& json, const Blackboard& blackboard) const
	{
		if (dataType.fromJSON != nullptr)
		{
			dataType.fromJSON(dataPtr, json, blackboard);
			return;
		}

		for (const DataTypeMemberVariable& member : dataType.memberVariables)
		{
			const DataType* memberDataType = Find(member.dataTypeID);
			if (memberDataType == nullptr)
			{
				continue;
			}

			if (!json.contains(member.name))
			{
				continue;
			}

			void* const memberPtr = PtrAdd(dataPtr, member.byteOffset);
			const nlohmann::json& memberJSON = json[member.name];
			LoadDataJSON(*memberDataType, memberPtr, memberJSON, blackboard);
		}
	}

	void DataTypeRegistry::LoadDataJSON(const DataTypeID dataTypeID, void* dataPtr, const nlohmann::json& json, const Blackboard& blackboard) const
	{
		const DataType* dataType = Find(dataTypeID);
		if (dataType == nullptr)
		{
			return;
		}

		LoadDataJSON(*dataType, dataPtr, json, blackboard);
	}

	nlohmann::json DataTypeRegistry::SaveDataJSON(const DataType& dataType, const void* dataPtr) const
	{
		if (dataType.toJSON != nullptr)
		{
			return dataType.toJSON(dataPtr, *this);
		}

		nlohmann::json json;

		for (const DataTypeMemberVariable& member : dataType.memberVariables)
		{
			const DataType* memberDataType = Find(member.dataTypeID);
			if (memberDataType == nullptr)
			{
				continue;
			}

			const void* const memberPtr = PtrAdd(dataPtr, member.byteOffset);
			json[member.name] = SaveDataJSON(*memberDataType, memberPtr);
		}

		return json;
	}

	nlohmann::json DataTypeRegistry::SaveDataJSON(const DataTypeID dataTypeID, const void* dataPtr) const
	{
		const DataType* dataType = Find(dataTypeID);
		if (dataType == nullptr)
		{
			return {};
		}
		
		return SaveDataJSON(*dataType, dataPtr);
	}
}