#include "Engine/Precompiled/EnginePch.hpp"
#include "EditorComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace CLX
{
#ifdef _EDITOR
    REGISTER_COMPONENT(EditorComponent, IsDefaultComponent{});
#else
    REGISTER_COMPONENT(EditorComponent);
#endif

    void RegisterEditorComponent() {}
}