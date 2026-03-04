#include "Engine/Precompiled/EnginePch.hpp"
#include "Engine/Reflection/DataTypeRegistry.hpp"
#include <External/imgui/imgui.h>

namespace CLX
{
	DataTypeRegistry::DataTypeRegistry()
	{
	}

	class DataTypeRegistryProxy final
	{
	public:

		DataTypeRegistryProxy(DataTypeRegistry*& aInstancePtr)
			: myInstancePtr(aInstancePtr)
		{
			aInstancePtr = new DataTypeRegistry();
		}

		~DataTypeRegistryProxy()
		{
			delete myInstancePtr;
			myInstancePtr = nullptr;
		}

	private:

		DataTypeRegistry*& myInstancePtr;
	};

	DataTypeRegistry& DataTypeRegistry::GetInstance()
	{
		static DataTypeRegistryProxy proxy(sInstance);
		return *sInstance;
	}

	void DataTypeRegistry::Destroy()
	{
		delete sInstance;
		sInstance = nullptr;
	}

	void DataTypeRegistry::Assert() const
	{
		for (auto& [_, dataType] : mDataTypes)
		{
			const bool hasEditFunction = dataType.viewAndEdit;
			const bool hasSaveFunction = dataType.toJSON;
			const bool hasLoadFunction = dataType.fromJSON;

			const short n = static_cast<short>(hasEditFunction) +
				static_cast<short>(hasSaveFunction) +
				static_cast<short>(hasLoadFunction);

			if (n != 0 && n != 3)
			{
				ASSERT(false && "DataType must have either all or none of the edit, save, and load functions defined.");
			}
		}
	}

	void* DataTypeRegistry::AddComponent(DataTypeID dataTypeID, ECS& ecs, EntityID entityID) const
	{
		return mDataTypes.at(dataTypeID).addComponentFunctionPointer(ecs, entityID);
	}

	void DataTypeRegistry::InplaceAllocateData(DataTypeID dataTypeID, void* dataPtr, const void* aDefaultValuePtr) const
	{
		mDataTypes.at(dataTypeID).inplaceAllocate(dataPtr, aDefaultValuePtr);
	}

	void DataTypeRegistry::CopyData(DataTypeID dataTypeID, void* aDestination, const void* aSource) const
	{
		mDataTypes.at(dataTypeID).copy(aDestination, aSource);
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

	static void* PtrAdd(void* aPtr, const std::size_t aOffset)
	{
		return reinterpret_cast<char*>(aPtr) + aOffset;
	}

	static const void* PtrAdd(const void* aPtr, const std::size_t aOffset)
	{
		return reinterpret_cast<const char*>(aPtr) + aOffset;
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
			return dataType.toJSON(dataPtr);
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
		if (dataType->toJSON != nullptr)
		{
			return dataType->toJSON(dataPtr);
		}

		return {};
	}
}