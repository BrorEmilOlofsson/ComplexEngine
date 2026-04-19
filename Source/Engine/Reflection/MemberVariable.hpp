#pragma once
#include <variant>
#include <string>
#include <any>
#include <optional>
#include <functional>

#include "Engine/Reflection/DataTypeID.hpp"
#include "Engine/Utility/ByteOffset.hpp"

namespace CLX
{

    struct FunctionMember final
    {
        std::function<void(void* ownerPtr, void* outPtr)> getFunction;
        std::function<void(void* ownerPtr, const void* newValuePtr)> setFunction;
    };


    using MemberType = std::variant<ByteOffset, FunctionMember>;

    struct DataTypeMemberVariable final
    {
        std::string name;
        std::string customName;
        DataTypeID dataTypeID = InvalidDataTypeID;
        MemberType memberType;
        bool shouldExpose = true;
        bool canEdit = true;
        std::any defaultValue;
        std::optional<std::any> minValue;
        std::optional<std::any> maxValue;
        std::optional<float> editorSpeed;
    };
}