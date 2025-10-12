#include "Editor/Precompiled/EditorPch.hpp"
#include "SetEntityTransformCommand.hpp"
#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Engine/Scene/SceneManager.hpp"

namespace Simple
{
	void SetEntityTransform(ECS& ecs, EntityID entityID, const Transform& transform)
	{
		TransformComponent* transformComponent = ecs.GetComponent<TransformComponent>(entityID);

		transformComponent->transform = transform;
	}

	void SetEntityTransformCommand::Execute() const
	{
		SetEntityTransform(*ecs, entityID, newTransform);
	}

	void SetEntityTransformCommand::Undo() const
	{
		SetEntityTransform(*ecs, entityID, oldTransform);
	}
}