#pragma once
#include <vector>
#include <algorithm>
#include <utility>
#include <ranges>

#include "Engine/Utility/Assert.hpp"
#include "Engine/Utility/RangeOf.hpp"

namespace CLX
{
    
    class DynamicBitset
    {
    private:

        DynamicBitset(std::vector<bool>&& bits) : mBits(std::move(bits)) {}

    public:

        DynamicBitset() = default;

        [[nodiscard]] static DynamicBitset FromRange(RangeOf<bool> auto&& range)
        {
            return DynamicBitset(range | std::ranges::to<std::vector<bool>>());
        }

        [[nodiscard]] static DynamicBitset FromSize(size_t size, bool defaultValue = false)
        {
            return DynamicBitset(std::vector<bool>(size, defaultValue));
        }

        [[nodiscard]] constexpr bool IsEmpty() const
        {
            return mBits.empty();
        }
        
        [[nodiscard]] constexpr size_t Count() const
        {
            return mBits.size();
        }

        [[nodiscard]] constexpr bool All() const
        {
            return std::ranges::all_of(mBits, [](bool bit) { return bit; });
        }

        [[nodiscard]] constexpr bool Any() const
        {
            return std::ranges::any_of(mBits, [](bool bit) { return bit; });
        }

        [[nodiscard]] constexpr bool None() const
        {
            return std::ranges::none_of(mBits, [](bool bit) { return bit; });
        }

        [[nodiscard]] constexpr auto operator[](size_t index)
        {
            ASSERT(index < mBits.size(), "Index out of bounds.");
            return mBits[index];
        }

        [[nodiscard]] constexpr bool operator[](size_t index) const
        {
            ASSERT(index < mBits.size(), "Index out of bounds.");
            return mBits[index];
        }

        [[nodiscard]] constexpr bool operator==(const DynamicBitset& other) const
        {
            return mBits == other.mBits;
        }

        [[nodiscard]] constexpr bool operator<(const DynamicBitset& other) const
        {
            return std::ranges::lexicographical_compare(mBits, other.mBits);
        }

    private:

        std::vector<bool> mBits;
    };

    constexpr DynamicBitset& operator&=(DynamicBitset& lhs, const DynamicBitset& rhs)
    {
        ASSERT(lhs.Count() == rhs.Count(), "Bitsets must be of the same size for AND operation.");
        for (size_t i = 0; i < lhs.Count(); ++i)
        {
            lhs[i] = lhs[i] && rhs[i];
        }
        return lhs;
    }

    [[nodiscard]] constexpr DynamicBitset operator&(DynamicBitset lhs, const DynamicBitset& rhs)
    {
        lhs &= rhs;
        return lhs;
    }

    constexpr DynamicBitset& operator|=(DynamicBitset& lhs, const DynamicBitset& rhs)
    {
        ASSERT(lhs.Count() == rhs.Count(), "Bitsets must be of the same size for OR operation.");
        for (size_t i = 0; i < lhs.Count(); ++i)
        {
            lhs[i] = lhs[i] || rhs[i];
        }
        return lhs;
    }

    [[nodiscard]] constexpr DynamicBitset operator|(DynamicBitset lhs, const DynamicBitset& rhs)
    {
        lhs |= rhs;
        return lhs;
    }

    constexpr DynamicBitset& operator^=(DynamicBitset& lhs, const DynamicBitset& rhs)
    {
        ASSERT(lhs.Count() == rhs.Count(), "Bitsets must be of the same size for XOR operation.");
        for (size_t i = 0; i < lhs.Count(); ++i)
        {
            lhs[i] = lhs[i] != rhs[i];
        }
        return lhs;
    }

    [[nodiscard]] constexpr DynamicBitset operator^(DynamicBitset lhs, const DynamicBitset& rhs)
    {
        lhs ^= rhs;
        return lhs;
    }

    [[nodiscard]] constexpr DynamicBitset operator~(DynamicBitset bitset)
    {
        for (size_t i = 0; i < bitset.Count(); ++i)
        {
            bitset[i] = !bitset[i];
        }
        return bitset;
    }
}