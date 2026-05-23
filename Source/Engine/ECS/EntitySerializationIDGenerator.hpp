#pragma once
#include <set>
#include <format>
#include "Engine/Utility/IDWrapper.hpp"
#include "Engine/Utility/Bounds.hpp"

namespace CLX
{

    using EntitySerializationID = IDWrapper<uint64_t>;

    class EntitySerializationIDGenerator
    {
    public:

        EntitySerializationIDGenerator() = default;


        [[nodiscard]] EntitySerializationID Generate()
        {
            Bounds<EntitySerializationID>* bounds = nullptr;
            if (mLowerBounds.GetExtent() == 0)
            {
                bounds = &mUpperBounds;
            }
            else
            {
                bounds = &mLowerBounds;
            }
           
            EntitySerializationID id = bounds->GetMin();
            bounds->SetMin(++id);
            mUsedIDs.insert(id);
            return id;
        }

        void SetUsedIDBounds(const Bounds<EntitySerializationID>& bounds)
        {
            auto min = bounds.GetMin();
            auto max = bounds.GetMax();
            mLowerBounds.SetMax(min);
            mUpperBounds.SetMin(max);
        }

        [[nodiscard]] Bounds<EntitySerializationID> GetUsedIDBounds() const
        {
            return Bounds<EntitySerializationID>::FromMinAndMax(mLowerBounds.GetMax(), mUpperBounds.GetMin());
        }

    private:

        std::set<EntitySerializationID> mUsedIDs;
        Bounds<EntitySerializationID> mLowerBounds{ Bounds<EntitySerializationID>::FromMinAndMax(EntitySerializationID{ 0 }, std::numeric_limits<EntitySerializationID>::max()) };
        Bounds<EntitySerializationID> mUpperBounds{ Bounds<EntitySerializationID>::FromMinAndMax(std::numeric_limits<EntitySerializationID>::max(), std::numeric_limits<EntitySerializationID>::max()) };
    };

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