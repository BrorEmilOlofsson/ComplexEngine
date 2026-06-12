#pragma once
#include "Engine/Math/Shapes/AABB2.hpp"
#include "Engine/Navmesh/NavmeshData.hpp"
#include "Engine/Utility/IDWrapper.hpp"
#include "Editor/Command/Core/EditorCommandTracker.hpp"

namespace CLX
{

    class Scene;
    class Camera;
    class InputState;

    struct NavmeshEditorData final
    {
        NavmeshVertexIndex selectedVertexIndex = InvalidID<NavmeshVertexIndex>();
        float vertexSelectDistance = 0.7f;

    };

    void ShowNavmeshEditor(Scene& scene, const Camera& camera, const AABB2i& renderRect, const InputState& input, NavmeshEditorData& navmeshEditorData, EditorCommandTracker& commandTracker);
}