#include "Engine/Precompiled/EnginePch.hpp"
#include "GraphicsFoundationCreator.hpp"

#include "Engine/OperatingSystem/OperatingSystem.hpp"

#ifdef _WIN32
#include "Engine/Graphics/DX11/DX11Foundation.hpp"
#include "Engine/Win/WinOperatingSystem.hpp"
#endif

namespace CLX
{

#ifdef _WIN32

    GraphicsFoundation CreateDX11Foundation(OperatingSystem* operatingSystem)
    {
        return GraphicsFoundation(DX11Foundation(operatingSystem->Cast<Win_OperatingSystem>()));
    }

#endif

    GraphicsFoundation CreateGraphicsFoundation(const eGraphicsAPI type, OperatingSystem* operatingSystem)
    {
        switch (type)
        {
        case eGraphicsAPI::DX11:
            return CreateDX11Foundation(operatingSystem);
        default: 
            throw std::runtime_error("Unsupported Graphics API");
        }
    }
}