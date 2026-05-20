#include "Engine/Precompiled/EnginePch.hpp"
#include "RotatingMovementSystem.hpp"
#include "Engine/ECSEngine/Components/RotatingMovementComponent.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Engine/Math/RotationMath.hpp"

namespace CLX
{

    void PerformRotation(Transform& transform, const UnitVector3f& rotationAxis, const float rotationSpeed, const float deltaTime)
	{
		const Degreesf degrees(deltaTime * rotationSpeed);
		const Matrix4x4f matrix = GetRotatedMatrixAroundAxis(transform.GetMatrix(), rotationAxis, degrees);
		transform.SetRotation(matrix.GetRotationMatrix());
	}

	void RotatingMovementSystem::Update(ECS& ecs, const float deltaTime, const Blackboard&)
	{
		ecs.ForEach([deltaTime](TransformComponent& transformComponent, const RotatingMovementComponent& rotatingMovement)
			{
                PerformRotation(transformComponent.transform, rotatingMovement.rotationAxis, rotatingMovement.rotationSpeed, deltaTime);
			});
	}
}