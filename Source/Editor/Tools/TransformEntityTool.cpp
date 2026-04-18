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

	void ShowEntityImGuizmo(ECS& ecs, const EntityID selectedEntityID, const eTransformMode transformMode, const Camera& camera, 
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

		static ImGuizmo::OPERATION operation = ImGuizmo::OPERATION::TRANSLATE;

		if (isCursorVisible && !input.IsKeyHeld(eInputKey::Ctrl))
		{
			if (input.IsKeyPressed(eInputKey::E))
			{
				operation = ImGuizmo::OPERATION::TRANSLATE;
			}
			else if (input.IsKeyPressed(eInputKey::R))
			{
				operation = ImGuizmo::OPERATION::ROTATE;
			}
			else if (input.IsKeyPressed(eInputKey::T))
			{
				operation = ImGuizmo::OPERATION::SCALE;
			}
		}

		const float snapV = useSnap ? snapValue : 0.f;
		const Vector3f gridSnapValues(snapV, snapV, snapV);

		const bool isManipulatingEntityTransform = ImGuizmo::Manipulate(
			view.GetDataPtr(),
			proj.GetDataPtr(),
			operation,
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

			switch (operation)
			{
			case ImGuizmo::OPERATION::TRANSLATE:
				transformComponent->transform.SetPosition(localTransform.GetPosition());
				break;
			case ImGuizmo::OPERATION::ROTATE:
				transformComponent->transform.SetMatrix(localTransform.GetMatrix());
				break;
			case ImGuizmo::OPERATION::SCALE:
				transformComponent->transform.SetScale(localTransform.GetScale());
				break;
			default:
				break;
			}
		}

		if (input.IsKeyReleased(eInputKey::LMB) && isDraggingEntity && selectedEntityID == setEntityTransformCommand.entityID)
		{
			isDraggingEntity = false;
			setEntityTransformCommand.newTransform = transformComponent->transform;
			setEntityTransformCommand.ecs = &ecs;
			commandTracker.RegisterCommand(EditorCommand(setEntityTransformCommand, "Set Entity Transform"));
		}
	}

	void TransformEntityTool::ShowEntityImGuizmo(ECS& ecs, const EntityID selectedEntityID, const eTransformMode transformMode, AABB2i renderRect,
		const bool useSnap, const float snapValue, const Camera& camera, const InputState& input, const bool isCursorVisible, EditorCommandTracker& commandTracker)
	{
		::CLX::ShowEntityImGuizmo(
			ecs, 
			selectedEntityID, 
			transformMode, 
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