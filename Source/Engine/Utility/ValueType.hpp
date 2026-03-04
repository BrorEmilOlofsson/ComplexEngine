#pragma once
#include <type_traits>

namespace CLX
{
    template<typename T>
    struct ValueType : std::type_identity<T> {};
}