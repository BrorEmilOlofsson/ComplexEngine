#pragma once
#include <limits>

namespace CLX
{

    struct ByteOffset final
    {
        std::size_t value = std::numeric_limits<std::size_t>::max();
    };


    template<typename MemberType, typename OwnerType>
    [[nodiscard]] constexpr ByteOffset GetByteOffset(MemberType OwnerType::* member)
    {
        return ByteOffset{ (std::size_t) & reinterpret_cast<const volatile char&>((((OwnerType*)0)->*member)) };
    }

    template<typename T>
    [[nodiscard]] constexpr T* operator+(T* ptr, const ByteOffset offset)
    {
        return static_cast<T*>(static_cast<char*>(ptr) + offset.value);
    }

    template<typename T>
    [[nodiscard]] constexpr const T* operator+(const T* ptr, const ByteOffset offset)
    {
        return static_cast<const T*>(static_cast<const char*>(ptr) + offset.value);
    }

    constexpr ByteOffset operator+=(ByteOffset& offset1, const ByteOffset offset2)
    {
        offset1.value += offset2.value;
        return offset1;
    }

    constexpr ByteOffset operator-=(ByteOffset& offset1, const ByteOffset offset2)
    {
        offset1.value -= offset2.value;
        return offset1;
    }

    constexpr ByteOffset operator+(ByteOffset offset1, const ByteOffset offset2)
    {
        return offset1 += offset2;
    }

    constexpr ByteOffset operator-(ByteOffset offset1, const ByteOffset offset2)
    {
        return offset1 -= offset2;
    }

    constexpr bool operator==(const ByteOffset offset1, const ByteOffset offset2)
    {
        return offset1.value == offset2.value;
    }
}