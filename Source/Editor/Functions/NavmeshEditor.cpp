#include "Editor/Precompiled/EditorPch.hpp"
#include "NavmeshEditor.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Editor/Functions/ECSEditorFunctions.hpp"

namespace CLX
{

    void ShowNavmeshEditor(Scene& scene, const Camera& camera, const AABB2i& renderRect, const InputState& input)
    {
        Navmesh& navmesh = scene.GetNavmesh();

        static NavmeshVertexIndex selectedVertexIndex = InvalidID<NavmeshVertexIndex>();
        
        if (input.IsKeyPressed(eInputKey::LMB))
        {
            auto raycastResult = navmesh.Raycast(scene.GetMouseRay());
            if (raycastResult)
            {
                selectedVertexIndex = navmesh.GetNode(raycastResult->mNodeIndex).mVertexIndices.front();
            }   
        }

        if (selectedVertexIndex == InvalidID<NavmeshVertexIndex>())
        {
            return;
        }

        auto& vertexPos = navmesh.GetNavmeshData().m3DVertices[selectedVertexIndex];

        Transform transform = Transform::FromPosition(vertexPos);

        auto result = ShowImGuizmo(
            camera,
            transform,
            eTransformMode::World,
            eTransformOperation::Translate,
            renderRect,
            10,
            false,
            0.f
        );

        if (result)
        {
            vertexPos = result->GetPosition();
        }
    }

}