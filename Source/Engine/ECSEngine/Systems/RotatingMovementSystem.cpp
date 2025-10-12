#include "Engine/Precompiled/EnginePch.hpp"
#include "RotatingMovementSystem.hpp"
#include "Engine/ECSEngine/Components/RotatingMovementComponent.hpp"
#include "Engine/ECS/ECS.hpp"

namespace Simple
{

	void RotatingMovementSystem::Update(ECS& ecs, const float deltaTime, const Blackboard&)
	{
		ecs.ForEach([deltaTime](TransformComponent& transformComponent, const RotatingMovementComponent& rotatingMovement)
			{
				const Degreesf degrees(deltaTime * rotatingMovement.rotationSpeed);
				const Matrix4x4f matrix = GetRotatedMatrixAroundAxis(transformComponent.transform.GetMatrix(), rotatingMovement.rotationAxis, degrees);
				transformComponent.transform.SetMatrix(matrix);
			});
	}
}