#include "Game/Precompiled/GamePch.hpp"
#include "PlayerMovementSystem.hpp"
#include "Game/ECS/Components/PlayerMovementComponent.hpp"
#include "Game/ECS/Components/PlanetComponent.hpp"
#include "Engine/ECSEngine/Components/CameraComponent.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformUtility.hpp"
#include "Engine/Utility/CollisionFunctions.hpp"
#include "Engine/Math/ShapeMath.hpp"
#include "Engine/Graphics/RenderState.hpp"

using namespace CLX;

void CreatePlayer(CLX::ECS& ecs)
{
	const CLX::EntityID entityID = ecs.CreateEntity();
	ecs.AddComponent<CLX::TransformComponent>(entityID);
	ecs.AddComponent<CLX::NameComponent>(entityID).name = "Player";
	ecs.AddComponent<CLX::MeshComponent>(entityID);
	ecs.AddComponent<PlayerMovementComponent>(entityID);

	const EntityID cameraEntityID = ecs.CreateEntity();
	Transform& transform = ecs.AddComponent<CLX::TransformComponent>(cameraEntityID).transform;
	transform.SetPosition(Point3f(0, 2, -5));
	transform.RotateOnAxis(UnitVector3f::Right(), Degreesf(-20.f));
	ecs.AddComponent<CLX::NameComponent>(cameraEntityID).name = "PlayerCamera";
	ecs.AddComponent<CameraComponent>(cameraEntityID);

	SetParentEntity(ecs, cameraEntityID, entityID);
}

void PlayerMovementSystem::BeginPlay(CLX::ECS&, const Blackboard&)
{
	//CreatePlayer(ecs);
}

static Vector3f GetMovementVector(const Transform& transform, const CLX::InputState& input)
{
	Vector3f moveVector;
	if (input.IsKeyHeld(eInputKey::W))
	{
		moveVector += UnitVector3f::Forward();
	}
	if (input.IsKeyHeld(eInputKey::A))
	{
		moveVector += UnitVector3f::Left();
	}
	if (input.IsKeyHeld(eInputKey::S))
	{
		moveVector += UnitVector3f::Backward();
	}
	if (input.IsKeyHeld(eInputKey::D))
	{
		moveVector += UnitVector3f::Right();
	}

	if (moveVector == Vector3f::Zero())
	{
		return Vector3f::Zero();
	}
	return ToNormalized(ToWorldSpace(moveVector, transform.GetMatrix()));

}

static EntityID FindClosestPlanetEntity(const ECS& ecs, const Transform& aPlayerTransform)
{
	EntityID closestEntityID = InvalidEntityID;
	float closestDistanceSquared = std::numeric_limits<float>::max();

	const Point3f playerPosition = aPlayerTransform.GetPosition();

	for (auto [entityID, planetComponent] : ecs.ViewUsingEntityID<PlanetComponent>())
	{
		const Transform planetWorldTransform = GetWorldTransform(ecs, entityID);
		if (planetWorldTransform.GetPosition() == playerPosition)
		{
			return entityID;
		}
		const Point3f planetPoint = planetWorldTransform.GetPosition() + GetUnitVector(planetWorldTransform.GetPosition(), playerPosition) * planetComponent.radius;

		const float distance = DistanceSquared(playerPosition, planetPoint);
		if (distance < closestDistanceSquared)
		{
			closestEntityID = entityID;
			closestDistanceSquared = distance;
		}
	}

	return closestEntityID;
}

static Vector3f GetGravityVector(const Transform& playerTransform, const PlanetComponent& planetComponent, const Transform& planetTransform)
{
	UnitVector3f diff;
	if (playerTransform.GetPosition() != planetTransform.GetPosition())
	{
		diff = GetUnitVector(playerTransform.GetPosition(), planetTransform.GetPosition());
	}
	return diff * planetComponent.gravityMagnitude;
}

static bool CheckPlanetCollision(const Transform& playerTransform, const PlanetComponent& planetComponent, const Transform& planetTransform)
{
	return DetectCollision(playerTransform.GetPosition(), Spheref::FromCenterAndRadius(planetTransform.GetPosition(), Radiusf(planetComponent.radius)));
}

static void HandleRotation(Transform& transform, const Transform& planetTransform)
{
	if (planetTransform.GetPosition() == transform.GetPosition())
	{
		return;
	}
	const UnitVector3f previousForward = transform.GetForwardVector();
	const UnitVector3f upVector = GetUnitVector(planetTransform.GetPosition(), transform.GetPosition());

	UnitVector3f forward = GetPerpendicularVector(upVector);
	if (previousForward != upVector)
	{
		const Planef plane = Planef::FromPointAndNormal(Point3f::Zero(), upVector);
		const Point3f proj = GetProjectedPoint(plane, Point3f::Zero() + previousForward);

		forward = GetUnitVector(Point3f::Zero(), proj);
	}

	transform.SetRotation(RotationMatrix3f::FromYZ(upVector, forward));
}

static void HandleMovement(PlayerMovementComponent& playerMovementComponent, Transform& playerTransform, const float deltaTime, const PlanetComponent& planetComponent, const Transform& planetTransform, const InputState& input)
{
	bool resetGravityVelocity = false;
	Vector3f movementVelocity = GetMovementVector(playerTransform, input) * playerMovementComponent.walkSpeed;
	Vector3f gravityVelocity;
	if (input.IsKeyPressed(eInputKey::Space))
	{
		playerMovementComponent.isInAir = true;
		UnitVector3f upVector = UnitVector3f::Up();
		if (playerTransform.GetPosition() != planetTransform.GetPosition())
		{
			upVector = GetUnitVector(planetTransform.GetPosition(), playerTransform.GetPosition());
		}
		gravityVelocity = upVector * playerMovementComponent.jumpForce;
	}
	else if (playerMovementComponent.isInAir)
	{
		if (CheckPlanetCollision(playerTransform, planetComponent, planetTransform))
		{
			playerMovementComponent.isInAir = false;
			resetGravityVelocity = true;
		}
		else
		{
			gravityVelocity = GetGravityVector(playerTransform, planetComponent, planetTransform);
		}
	}


	if (resetGravityVelocity)
	{
		playerMovementComponent.gravityVelocity = Vector3f::Zero();
	}
	else
	{
		playerMovementComponent.gravityVelocity += gravityVelocity * deltaTime;
	}

	playerTransform.MovePosition(movementVelocity * deltaTime);
	playerTransform.MovePosition(playerMovementComponent.gravityVelocity * deltaTime);

	HandleRotation(playerTransform, planetTransform);
	if (!playerMovementComponent.isInAir)
	{
		UnitVector3f upVector = UnitVector3f::Up();
		if (playerTransform.GetPosition() != planetTransform.GetPosition())
		{
			upVector = GetUnitVector(planetTransform.GetPosition(), playerTransform.GetPosition());
		}
		playerTransform.SetPosition(planetTransform.GetPosition() + upVector * planetComponent.radius);
	}
}

static void RenderPlanets(const ECS& ecs, RenderList& renderList)
{
	for (auto [entityID, planetComponent] : ecs.ViewUsingEntityID<PlanetComponent>())
	{
		Transform planetTransform = GetWorldTransform(ecs, entityID);
		renderList.AddSphere(DrawSphere{ Spheref::FromCenterAndRadius(planetTransform.GetPosition(), Radiusf(planetComponent.radius)), Colors::Yellow });
	}
}

void PlayerMovementSystem::Update(CLX::ECS& ecs, const float deltaTime, const Blackboard& blackboard)
{
	auto playerMovementView = ecs.View<PlayerMovementComponent, TransformComponent>();

	const InputState& input = blackboard.Get<Key_InputState>();

	for (auto [playerMovementComponent, transformComponent] : playerMovementView)
	{
		const EntityID planetID = FindClosestPlanetEntity(ecs, transformComponent.transform);
		if (planetID == InvalidEntityID)
		{
			continue;
		}
		const PlanetComponent& planetComponent = *ecs.GetComponent<PlanetComponent>(planetID);
		const Transform& planetTransform = GetWorldTransform(ecs, planetID);

		HandleMovement(
			playerMovementComponent, 
			transformComponent.transform, 
			deltaTime, 
			planetComponent, 
			planetTransform, 
			input
		);
	}

	RenderPlanets(ecs, blackboard.Get<Key_SceneRenderState>().GetRenderList());
}

void PlayerMovementSystem::EditorUpdate(CLX::ECS& ecs, const Blackboard& blackboard)
{
	RenderPlanets(ecs, blackboard.Get<Key_SceneRenderState>().GetRenderList());
}