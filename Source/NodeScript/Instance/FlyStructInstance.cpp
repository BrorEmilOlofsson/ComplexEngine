#include "FlyStructInstance.hpp"
#include "../DataType/FlyStruct.hpp"

namespace FLY_NAMESPACE
{
    StructInstance::StructInstance(const Struct& s)
        : mStruct(&s)
        , mVariableContainerInstance(s.GetVariableContainer())
    {
    }
}
