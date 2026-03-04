#pragma once
#include "Editor/Command/Commands/SetEntityTransformCommand.hpp"
#include "Engine/Math/TransformMode.hpp"
#include "Engine/Utility/Camera.hpp"
#include "Engine/Math/Shapes/AABB2.hpp"

namespace Simple
{

	void ShowEntityImGuizmo(ECS& ecs, const EntityID selectedEntityID, const eTransformMode transformMode, const Camera& camera, AABB2i renderRect,
		const bool useSnap, const float snapValue, bool& isDraggingEntity, SetEntityTransformCommand& setEntityTransformCommand,
		const InputState& input, bool isCursorVisible, EditorCommandTracker& commandTracker);

	class TransformEntityTool final
	{
	public:

		TransformEntityTool() = default;

		void ShowEntityImGuizmo(ECS& ecs, const EntityID selectedEntityID, const eTransformMode transformMode, AABB2i renderRect, const bool useSnap, const float snapValue, const Camera& camera, 
			const InputState& input, bool isCursorVisible, EditorCommandTracker& commandTracker);

	private:

		SetEntityTransformCommand mSetEntityTransformCommand;
		bool mIsDraggingEntity = false;
	};
}