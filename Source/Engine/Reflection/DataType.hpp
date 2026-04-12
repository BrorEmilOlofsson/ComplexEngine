#pragma once
#include <string>
#include <vector>
#include <type_traits>
#include <limits>

#include <External/nlohmann/json.hpp>

#include "Engine/Reflection/DataTypeID.hpp"
#include "Engine/Reflection/MemberVariable.hpp"
#include "Engine/Reflection/ViewAndEditResult.hpp"
#include "Engine/Utility/Blackboard.hpp"
#include "Engine/Utility/FundamentalFunctionDefinitions.hpp"

namespace CLX
{

    class DataType final
    {
    public:
        std::string name;
        std::string prettyName;
        std::vector<DataTypeMemberVariable> memberVariables;

        ViewAndEditResult(*viewAndEdit)(void* dataPtr, const Blackboard& blackboard, const DataTypeMemberVariable* member) = nullptr;
        nlohmann::json(*toJSON)(const void* dataPtr, const class DataTypeRegistry& dataTypeRegistry) = nullptr;
        void (*fromJSON)(void* dataPtr, const nlohmann::json& json, const Blackboard& blackboard) = nullptr;

        InplaceConstructFunction inplaceConstruct;
        DestroyFunction destroy;
        CopyFunction copy;
        MoveFunction move;
        SwapFunction swap;

        std::size_t size = std::numeric_limits<std::size_t>::max();
        std::size_t alignment = std::numeric_limits<std::size_t>::max();
        std::reference_wrapper<const std::type_info> typeInfo;
        DataTypeID containingValueDataTypeID = InvalidDataTypeID;
        bool isComponent = false;
    };

}