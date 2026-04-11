#pragma once
#include <string>
#include <vector>
#include <type_traits>

#include <External/nlohmann/json.hpp>

#include "Engine/Reflection/DataTypeID.hpp"
#include "Engine/Reflection/MemberVariable.hpp"
#include "Engine/Reflection/ViewAndEditResult.hpp"
#include "Engine/Utility/Blackboard.hpp"

namespace CLX
{

    using InPlaceAllocateFunction = void(*)(void* data, const void* defaultValuePtr);
    using DestroyFunction = void(*)(void* data);
    using CopyFunction = void (*)(void* destinationPtr, const void* sourcePtr);
    using SwapFunction = void (*)(void* dataPtr1, void* dataPtr2);

    class DataType final
    {
    public:
        std::string name;
        std::string prettyName;
        std::vector<DataTypeMemberVariable> memberVariables;

        ViewAndEditResult(*viewAndEdit)(void* dataPtr, const Blackboard& blackboard, const DataTypeMemberVariable* member) = nullptr;
        nlohmann::json(*toJSON)(const void* dataPtr, const class DataTypeRegistry& dataTypeRegistry) = nullptr;
        void (*fromJSON)(void* dataPtr, const nlohmann::json& json, const Blackboard& blackboard) = nullptr;

        InPlaceAllocateFunction inplaceAllocate = nullptr;
        DestroyFunction destroy = nullptr;
        CopyFunction copy = nullptr;
        SwapFunction swap = nullptr;

        std::size_t size = std::numeric_limits<std::size_t>::max();
        std::size_t alignment = std::numeric_limits<std::size_t>::max();
        std::reference_wrapper<const std::type_info> typeInfo;
        DataTypeID containingValueDataTypeID = InvalidDataTypeID;
        bool isComponent = false;
    };

}