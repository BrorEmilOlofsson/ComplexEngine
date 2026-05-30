#pragma once
#include <cstdint>
#include <format>
#include "Engine/Utility/IDWrapper.hpp"

namespace CLX
{

    using EntitySerializationID = IDWrapper<uint64_t, struct EntitySerializationIDTag>;

}

namespace std
{
    template<>
    struct formatter<CLX::EntitySerializationID> : std::formatter<CLX::EntitySerializationID::value_type>
    {
        auto format(const CLX::EntitySerializationID& id, auto& ctx) const
        {
            return std::formatter<CLX::EntitySerializationID::value_type>::format(id.id, ctx);
        }
    };
}