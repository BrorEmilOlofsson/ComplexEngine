#include "Editor/Precompiled/EditorPch.hpp"
#include "TransformEntityTool.hpp"
#include "Editor/Command/Core/EditorCommandTracker.hpp"
#include "Editor/Utility/EditorUtilities.hpp"
#include "Engine/ECSEngine/Utility/ECSUtilityFunctions.hpp"
#include <External/imgui/imguizmo/imguizmo.h>

namespace CLX
{

    constexpr ImGuizmo::MODE ToImGuizmoMode(eTransformMode transformMode)
    {
        return transformMode == eTransformMode::Local ? ImGuizmo::MODE::LOCAL : ImGuizmo::MODE::WORLD;
    }

    constexpr ImGuizmo::OPERATION ToImGuizmoOperation(eTransformOperation operation)
    {
        switch (operation)
        {
        case eTransformOperation::None:
            return static_cast<ImGuizmo::OPERATION>(0);
        case eTransformOperation::TranslateX:
            return ImGuizmo::OPERATION::TRANSLATE_X;
        case eTransformOperation::TranslateY:
            return ImGuizmo::OPERATION::TRANSLATE_Y;
        case eTransformOperation::TranslateZ:
            return ImGuizmo::OPERATION::TRANSLATE_Z;
        case eTransformOperation::RotateX:
            return ImGuizmo::OPERATION::ROTATE_X;
        case eTransformOperation::RotateY:
            return ImGuizmo::OPERATION::ROTATE_Y;
        case eTransformOperation::RotateZ:
            return ImGuizmo::OPERATION::ROTATE_Z;
        case eTransformOperation::ScaleX:
            return ImGuizmo::OPERATION::SCALE_X;
        case eTransformOperation::ScaleY:
            return ImGuizmo::OPERATION::SCALE_Y;
        case eTransformOperation::ScaleZ:
            return ImGuizmo::OPERATION::SCALE_Z;
        case eTransformOperation::Translate:
            return ImGuizmo::OPERATION::TRANSLATE;
        case eTransformOperation::Rotate:
            return ImGuizmo::OPERATION::ROTATE;
        case eTransformOperation::Scale:
            return ImGuizmo::OPERATION::SCALE;
        case eTransformOperation::All:
            return static_cast<ImGuizmo::OPERATION>(~0);
        default:
            ASSERT_NEW(false, "Invalid transform operation.");
            return static_cast<ImGuizmo::OPERATION>(0);
        }
    }

    void ShowEntityImGuizmo(ECS& ecs, const EntityID selectedEntityID, const eTransformMode transformMode, eTransformOperation& transformOperation, const Camera& camera,
        const AABB2i renderRect, const bool useSnap, const float snapValue, bool& isDraggingEntity,
        SetEntityTransformCommand& setEntityTransformCommand, const InputState& input, const bool isCursorVisible, EditorCommandTracker& commandTracker)
    {
        if (selectedEntityID == InvalidEntityID)
        {
            return;
        }

        ImGuizmo::AllowAxisFlip(false);

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
        //const Point2f topLeft = Point2f(GetTopLeftCorner(renderRect));
        ImGuizmo::SetRect((float)renderRect.GetMin().x, (float)renderRect.GetMin().y, (float)renderRect.GetExtent().x, (float)renderRect.GetExtent().y);

        TransformComponent* transformComponent = ecs.GetComponent<TransformComponent>(selectedEntityID);
        ASSERT_NEW(transformComponent != nullptr, "Selected entity does not have a TransformComponent.");

        Matrix4x4f objectMatrix = GetEntityWorldTransform(ecs, selectedEntityID).GetMatrix();
        const Matrix4x4f view = camera.GetViewMatrix();
        const Matrix4x4f proj = camera.GetProjectionMatrix();


        if (isCursorVisible && !input.IsKeyHeld(eInputKey::Ctrl))
        {
            if (input.IsKeyPressed(eInputKey::E))
            {
                transformOperation = eTransformOperation::Translate;
            }
            else if (input.IsKeyPressed(eInputKey::R))
            {
                transformOperation = eTransformOperation::Rotate;
            }
            else if (input.IsKeyPressed(eInputKey::T))
            {
                transformOperation = eTransformOperation::Scale;
            }
        }

        const ImGuizmo::OPERATION guizmoOperation = ToImGuizmoOperation(transformOperation);

        const float snapV = useSnap ? snapValue : 0.f;
        const Vector3f gridSnapValues(snapV, snapV, snapV);

        const bool isManipulatingEntityTransform = ImGuizmo::Manipulate(
            view.GetDataPtr(),
            proj.GetDataPtr(),
            guizmoOperation,
            ToImGuizmoMode(transformMode),
            objectMatrix.GetDataPtr(),
            nullptr,
            &gridSnapValues.x
        );

        if (isManipulatingEntityTransform && input.IsKeyDown(eInputKey::LMB) && !isDraggingEntity)
        {
            isDraggingEntity = true;
            setEntityTransformCommand.entityID = selectedEntityID;
            setEntityTransformCommand.oldTransform = transformComponent->transform;
        }

        if (isManipulatingEntityTransform)
        {
            const Transform localTransform = GetEntityLocalTransform(ecs, selectedEntityID, Transform::FromMatrix(objectMatrix));

            if (HasAnyFlag(transformOperation, eTransformOperation::Translate))
            {
                transformComponent->transform.SetPosition(localTransform.GetPosition());
            }
            else if (HasAnyFlag(transformOperation, eTransformOperation::Rotate))
            {
                transformComponent->transform.SetMatrix(localTransform.GetMatrix());
            }
            else if (HasAnyFlag(transformOperation, eTransformOperation::Scale))
            {
                transformComponent->transform.SetScale(localTransform.GetScale());
            }
        }

        if (input.IsKeyReleased(eInputKey::LMB) && isDraggingEntity && selectedEntityID == setEntityTransformCommand.entityID)
        {
            isDraggingEntity = false;
            setEntityTransformCommand.newTransform = transformComponent->transform;
            setEntityTransformCommand.ecs = &ecs;
            commandTracker.ExecuteCommand(EditorCommand(setEntityTransformCommand, "Set Entity Transform"));
        }
    }

    void TransformEntityTool::ShowEntityImGuizmo(ECS& ecs, const EntityID selectedEntityID, const eTransformMode transformMode, eTransformOperation& transformOperation, AABB2i renderRect,
        const bool useSnap, const float snapValue, const Camera& camera, const InputState& input, const bool isCursorVisible, EditorCommandTracker& commandTracker)
    {
        ::CLX::ShowEntityImGuizmo(
            ecs,
            selectedEntityID,
            transformMode,
            transformOperation,
            camera,
            renderRect,
            useSnap,
            snapValue,
            mIsDraggingEntity,
            mSetEntityTransformCommand,
            input,
            isCursorVisible,
            commandTracker);
    }
}