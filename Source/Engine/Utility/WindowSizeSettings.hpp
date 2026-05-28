#pragma once
#include <variant>
#include "Engine/Math/Dimension2.hpp"

namespace CLX
{
    struct FullScreen {};
    struct WindowedFullScreen {};

    using WindowSizeSettings = std::variant<Dimension2u, WindowedFullScreen, FullScreen>;
}