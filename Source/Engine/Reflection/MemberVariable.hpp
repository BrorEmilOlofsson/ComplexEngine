#pragma once
#include <variant>
#include <string>
#include <any>
#include <optional>
#include <functional>

#include "Engine/Reflection/DataTypeID.hpp"

namespace CLX
{

    struct ByteOffset final
    {
        std::size_t value = std::numeric_limits<std::size_t>::max();
    };

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