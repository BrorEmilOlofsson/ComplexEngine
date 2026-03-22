#pragma once

namespace CLX
{
    struct EditorComponent final
    {
        bool isDestroyed = false;
    };

    void RegisterEditorComponent();
}