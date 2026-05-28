#pragma once
#include <cstdint>
#include "Engine/Utility/IDWrapper.hpp"

namespace CLX
{

    using AudioChannelID = IDWrapper<uint32_t, struct AudioChannelIDTag>;

}