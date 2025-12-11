#pragma once
#include <type_traits>

namespace Simple
{
    template<typename T>
    struct ValueType : std::type_identity<T> {};
}