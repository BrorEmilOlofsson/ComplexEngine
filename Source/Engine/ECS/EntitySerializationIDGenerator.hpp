#pragma once
#include <unordered_set>
#include <optional>
#include "Engine/Utility/Bounds.hpp"
#include "EntitySerializationID.hpp"

namespace CLX
{

    class EntitySerializationIDGenerator
    {
    public:

        EntitySerializationIDGenerator() = default;

        [[nodiscard]] EntitySerializationID Generate()
        {
            EntitySerializationID id;
            if (mUsedBounds.GetMin() > 0)
            {
                id = EntitySerializationID{ mUsedBounds.GetMin() - 1 };
                mUsedBounds.SetMin(id);
            }
            else if (mUsedBounds.GetMax() < std::numeric_limits<uint64_t>::max())
            {
                id = EntitySerializationID{ mUsedBounds.GetMax() + 1 };
                mUsedBounds.SetMax(id);
            }
            else
            {
                ASSERT(false);
            }

            mUsedIDs.insert(id);
            return id;
        }

        void MarkIDAsUsed(const EntitySerializationID id)
        {
            mUsedBounds.ExpandToContain(id);
            auto [_, inserted] = mUsedIDs.insert(id);
            ASSERT(inserted);
        }

        [[nodiscard]] bool IsIDUsed(const EntitySerializationID id) const
        {
            return mUsedIDs.contains(id);
        }

        void SetUsedIDBounds(const Bounds<uint64_t>& bounds)
        {
            mUsedBounds = bounds;
        }

        [[nodiscard]] std::optional<Bounds<uint64_t>> GetUsedIDBounds() const
        {
            if (mUsedBounds.GetExtent() == 0)
            {
                return std::nullopt;
            }

            return mUsedBounds;
        }

    private:

        std::unordered_set<EntitySerializationID> mUsedIDs;
        Bounds<uint64_t> mUsedBounds = Bounds<uint64_t>::FromMinAndMax(0, 0);
    };

}
