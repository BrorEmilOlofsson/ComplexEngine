#pragma once
#include <typeindex>
#include <External/nlohmann/json.hpp>

#include "Engine/Reflection/DataTypeID.hpp"

namespace CLX
{

    struct JsonAny final
    {
        DataTypeID dataTypeID;
        nlohmann::json json;
    };
}