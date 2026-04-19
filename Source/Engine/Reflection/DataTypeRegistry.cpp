#include "Engine/Precompiled/EnginePch.hpp"
#include "Engine/Reflection/DataTypeRegistry.hpp"
#include <External/imgui/imgui.h>
#include "PropertyPath.hpp"
#include "Engine/Utility/Memory/GenericBufferArena.hpp"

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
        mDataTypes.at(dataTypeID).inplaceConstruct(dataPtr, defaultValuePtr);
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

    void DataTypeRegistry::MoveData(DataTypeID dataTypeID, void* destination, void* source) const
    {
        mDataTypes.at(dataTypeID).move(destination, source);
    }

    bool DataTypeRegistry::EqualsData(DataTypeID dataTypeID, const void* dataPtr1, const void* dataPtr2) const
    {
        if (EqualsFunction equalsFunction = mDataTypes.at(dataTypeID).equals)
        {
            return equalsFunction(dataPtr1, dataPtr2);
        }

        for (auto member : mDataTypes.at(dataTypeID).memberVariables)
        {
            const DataType* memberDataType = Find(member.dataTypeID);
            ASSERT(memberDataType != nullptr);
            const void* memberDataPtr1 = dataPtr1 + std::get<ByteOffset>(member.memberType);
            const void* memberDataPtr2 = dataPtr2 + std::get<ByteOffset>(member.memberType);
            if (!EqualsData(member.dataTypeID, memberDataPtr1, memberDataPtr2))
            {
                return false;
            }
        }

        return true;

    }

    size_t DataTypeRegistry::GetDataTypeSize(DataTypeID dataTypeID) const
    {
        return mDataTypes.at(dataTypeID).size;
    }

    InplaceConstructFunction DataTypeRegistry::GetInplaceConstructFunction(DataTypeID dataTypeID) const
    {
        return mDataTypes.at(dataTypeID).inplaceConstruct;
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

    ViewAndEditResult DataTypeRegistry::ViewAndEditData(const DataTypeID dataTypeID, void* const dataPtr, const Blackboard& blackboard, const DataTypeMemberVariable* memberData) const
    {
        const DataType* dataType = Find(dataTypeID);
        if (dataType == nullptr)
        {
            return ViewAndEditResult{};
        }
        if (dataType->viewAndEdit != nullptr)
        {
            return dataType->viewAndEdit(dataPtr, blackboard, memberData);
        }

        ViewAndEditResult viewAndEditResult;
        for (auto& member : dataType->memberVariables)
        {
            if (!member.shouldExpose)
            {
                continue;
            }

            ImGui::BeginDisabled(!member.canEdit);
            if (std::holds_alternative<ByteOffset>(member.memberType))
            {
                void* memberDataPtr = dataPtr + std::get<ByteOffset>(member.memberType);
                Blackboard newBlackboard = blackboard;
                newBlackboard.Insert<Key_VariableName>(member.customName);
                PropertyPath& propertyPath = newBlackboard.Get<Key_CurrentPropertyPath>();
                propertyPath.elements.push_back(member.name);
                viewAndEditResult |= ViewAndEditData(member.dataTypeID, memberDataPtr, newBlackboard, &member);
            }
            else
            {
                const DataType* memberDataType = Find(member.dataTypeID);
                ASSERT(memberDataType != nullptr);
                void* ownerPtr = dataPtr;
                GenericBufferArena<1028> arena;
                void* allocatedDataPtr = arena.AllocateTypeErased(memberDataType->size, memberDataType->alignment, memberDataType->destroy, memberDataType->copy, memberDataType->type);
                FunctionMember functionMember = std::get<FunctionMember>(member.memberType);
                functionMember.getFunction(ownerPtr, allocatedDataPtr);
                viewAndEditResult |= ViewAndEditData(member.dataTypeID, allocatedDataPtr, blackboard);

            }

            if (viewAndEditResult.isActive)
            {
                //blackboard.Get<Key_ActivePropertyPath>() = blackboard.Get<Key_CurrentPropertyPath>();
            }

            ImGui::EndDisabled();
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

            void* const memberPtr = dataPtr + std::get<ByteOffset>(member.memberType);
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

            const void* const memberPtr = dataPtr + std::get<ByteOffset>(member.memberType);
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