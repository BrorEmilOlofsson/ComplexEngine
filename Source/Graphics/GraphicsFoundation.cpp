#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "GraphicsFoundation.hpp"

namespace Simple
{

    RenderContext GraphicsFoundation::CreateRenderContext(const Vector2ui& size)
    {
        return mConcept->CreateRenderContext(size);
    }
}