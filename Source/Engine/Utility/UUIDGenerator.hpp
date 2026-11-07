#pragma once
#pragma warning (disable : 4244)
#include <External/uuid_v4/uuid_v4.h>
#pragma warning (default : 4244)
#include <random>
#include <string>

namespace CLX
{

    struct UUID
    {
        UUID() = delete;

        explicit UUID(const UUIDv4::UUID& uuid)
            : mUUID(uuid)
        {
        }

        [[nodiscard]] const UUIDv4::UUID& Get() const
        {
            return mUUID;
        }

        std::string ToString() const
        {
            return mUUID.str();
        }

        friend bool operator==(const UUID& lhs, const UUID& rhs) { return lhs.Get() == rhs.Get(); }

    private:

        UUIDv4::UUID mUUID;
    };

    class UUIDGenerator final
    {
    public:

        UUIDGenerator() = default;

        [[nodiscard]] UUID Generate()
        {
            return UUID(mGenerator.getUUID());
        }

    private:

        UUIDv4::UUIDGenerator<std::mt19937_64> mGenerator;
    };
}