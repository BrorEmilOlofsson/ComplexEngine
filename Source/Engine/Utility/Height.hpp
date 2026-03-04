#pragma once
#include "Engine/Utility/StrongType.hpp"
#include "Engine/Utility/Clamper.hpp"

namespace CLX
{
    template<typename T>
    using Height = StrongAssertedTypeNew<T, PositiveOrZeroAsserter<T>, struct HeightTag, EquatableTrait, ComparableTrait>;

    using Heightf = Height<float>;
    using Heightd = Height<double>;
}