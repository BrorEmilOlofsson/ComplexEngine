#pragma once
#include "Engine/Graphics/GraphicsFoundation.hpp"

namespace CLX
{

    enum class eGraphicsAPI
    {
        DX11,
    };


#ifdef _WIN32

    [[nodiscard]] GraphicsFoundation CreateDX11Foundation();

#endif

    [[nodiscard]] GraphicsFoundation CreateGraphicsFoundation(eGraphicsAPI type);
}