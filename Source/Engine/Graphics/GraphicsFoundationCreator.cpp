#include "Engine/Precompiled/EnginePch.hpp"
#include "GraphicsFoundationCreator.hpp"

#ifdef _WIN32
#include "Engine/Graphics/DX11/DX11Foundation.hpp"
#endif

namespace CLX
{

#ifdef _WIN32

    GraphicsFoundation CreateDX11Foundation()
    {
        return GraphicsFoundation(DX11Foundation());
    }

#endif

    GraphicsFoundation CreateGraphicsFoundation(const eGraphicsAPI type)
    {
        switch (type)
        {
        case eGraphicsAPI::DX11:
            return CreateDX11Foundation();
        default: 
            throw std::runtime_error("Unsupported Graphics API");
        }
    }
}