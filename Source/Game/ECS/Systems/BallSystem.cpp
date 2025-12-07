#include "Game/Precompiled/GamePch.hpp"
#include "BallSystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Game/ECS/Components/PhysicsComponent.hpp"
#include "Game/ECS/Components/BallComponent.hpp"
#include "Game/ECS/Components/PlayerShootingComponent.hpp"
#include "Engine/ECSEngine/Components/AllEngineComponents.hpp"
#include "Utility/Math/GeometryMath.hpp"
#include "Engine/Physics/Physics.hpp"
#include "Utility/Intersection.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Scene/Scene.hpp"

using namespace Simple;


constexpr Point3f startPoint = Point3f(-3, 1, -18);

static void CreateBall(Simple::ECS& ecs)
{

	Simple::EntityID entityID = ecs.CreateEntity();
	Simple::TransformComponent& transformComponent = ecs.AddComponent<Simple::TransformComponent>(entityID);
	ecs.AddComponent<Simple::NameComponent>(entityID).name = "Ball";
	PhysicsComponent& physicsComponent = ecs.AddComponent<PhysicsComponent>(entityID);
	ecs.AddComponent<BallComponent>(entityID);

	physicsComponent.physicsObject.SetMass(Simple::Physics::Massf(410.0f));

	const Simple::Circumferencef circumference(0.7f);
	const Simple::Radiusf ballRadius = Simple::ToRadius(circumference);
	const Simple::Spheref ballSphere = Spheref::FromCenterAndRadius(startPoint, ballRadius.Value());

	physicsComponent.physicsObject.SetShape(ballSphere);
	transformComponent.transform.SetPosition(ballSphere.GetCenter());
}

constexpr float barHeight = 2.4384f;
constexpr float barWidth = 7.3152f;

static EntityID CreateBar(Simple::ECS& ecs)
{
	const Simple::EntityID entityID = ecs.CreateEntity();
	Simple::TransformComponent& transformComponent = ecs.AddComponent<Simple::TransformComponent>(entityID);
	ecs.AddComponent<Simple::NameComponent>(entityID).name = "Bar";
	PhysicsComponent& physicsComponent = ecs.AddComponent<PhysicsComponent>(entityID);

	constexpr Radiusf barRadius = Simple::ToRadius(Diameterf(0.12f));

	constexpr Simple::Cylinderf barShape(Point3f(0, barHeight, 0), barRadius.Value(), Simple::UnitVector3f::Right(), barWidth);

	physicsComponent.physicsObject.SetColor(Simple::Colors::Aqua);
	physicsComponent.physicsObject.SetType(Simple::Physics::ePhysicsObjectType::Static);
	physicsComponent.physicsObject.SetMass(Simple::Physics::Massf(1000.0f));
	physicsComponent.physicsObject.SetShape(barShape);
	transformComponent.transform.SetPosition(barShape.GetCenter());
	return entityID;
}

static EntityID CreatePost(Simple::ECS& ecs)
{
	Simple::EntityID entityID = ecs.CreateEntity();
	Simple::TransformComponent& transformComponent = ecs.AddComponent<Simple::TransformComponent>(entityID);
	ecs.AddComponent<Simple::NameComponent>(entityID).name = "Post";
	PhysicsComponent& physicsComponent = ecs.AddComponent<PhysicsComponent>(entityID);

	constexpr Radiusf postRadius = Simple::ToRadius(Simple::Diameterf(0.12f));

	constexpr Simple::Cylinderf postShape(Point3f(0, 0, 0), postRadius.Value(), Simple::UnitVector3f::Up(), barHeight);

	physicsComponent.physicsObject.SetColor(Simple::Colors::Aqua);
	physicsComponent.physicsObject.SetType(Simple::Physics::ePhysicsObjectType::Static);
	physicsComponent.physicsObject.SetMass(Simple::Physics::Massf(1000.0f));
	physicsComponent.physicsObject.SetShape(postShape);
	transformComponent.transform.SetPosition(postShape.GetCenter());
	return entityID;
}

static void CreateGoal(Simple::ECS& ecs)
{
	const EntityID barID = CreateBar(ecs);

	const Point3f barPos = ecs.GetComponent<TransformComponent>(barID)->transform.GetPosition();

	{
		const EntityID leftPostID = CreatePost(ecs);

		const Point3f leftPostPos = barPos + UnitVector3f::Left() * barWidth * 0.5f + UnitVector3f::Down() * barHeight * 0.5f;
		ecs.GetComponent<TransformComponent>(leftPostID)->transform.SetPosition(leftPostPos);
		ecs.GetComponent<PhysicsComponent>(leftPostID)->physicsObject.SetPosition(leftPostPos);
	}
	{
		const EntityID rightPostID = CreatePost(ecs);
		const Point3f rightPostPos = barPos + UnitVector3f::Right() * barWidth * 0.5f + UnitVector3f::Down() * barHeight * 0.5f;
		ecs.GetComponent<TransformComponent>(rightPostID)->transform.SetPosition(rightPostPos);
		ecs.GetComponent<PhysicsComponent>(rightPostID)->physicsObject.SetPosition(rightPostPos);
	}
}

static void CreatePlayer(Simple::ECS& ecs)
{
	Simple::EntityID playerEntityID = ecs.CreateEntity();
	ecs.AddComponent<Simple::TransformComponent>(playerEntityID);
	ecs.AddComponent<Simple::NameComponent>(playerEntityID).name = "Player";

	ecs.AddComponent<PlayerShootingComponent>(playerEntityID);
}

void BallSystem::BeginPlay(Simple::ECS& ecs, const Simple::Blackboard&)
{
	CreateBall(ecs);
	CreateGoal(ecs);
	CreatePlayer(ecs);
}

static void UpdateBalls(Simple::ECS& ecs, const float deltaTime, const Camera& camera, const InputState& input, const Scene& currentScene)
{
	auto playerView = ecs.View<PlayerShootingComponent>();

	if (playerView.IsEmpty())
	{
		return;
	}

	PlayerShootingComponent& playerComponent = *playerView.begin();

	if (input.IsKeyPressed(Simple::eInputKey::R))
	{
		auto view = ecs.View<PhysicsComponent, TransformComponent, BallComponent>();

		for (auto [physicsComponent, transformComponent, ballComponent] : view)
		{
			physicsComponent.physicsObject.SetPosition(startPoint);
			transformComponent.transform.SetPosition(startPoint);

			physicsComponent.physicsObject.SetVelocity(Simple::Physics::Velocityf());
			physicsComponent.physicsObject.SetCurve(0.f);
		}
	}

	auto view = ecs.View<PhysicsComponent, BallComponent>();

	for (auto [physicsComponent, ballComponent] : view)
	{
		Simple::Physics::PhysicsObject& obj = physicsComponent.physicsObject;

		const Vector3f velocityVec = obj.GetVelocity().Value();

		if (Vector3f(velocityVec.x, 0.f, velocityVec.z) == Vector3f::Zero())
		{
			continue;
		}

		const Vector3f curveDir = Cross(UnitVector3f(velocityVec.x, 0, velocityVec.z), UnitVector3f::Up()) * physicsComponent.physicsObject.GetCurve();

		obj.AddVelocity(Simple::Physics::Velocityf(curveDir * deltaTime));

	}
	if (!input.IsKeyPressed(eInputKey::LMB))
	{
		return;
	}

	const UnitVector3f cameraForward = [&camera]()
		{
			const UnitVector3f cameraForward = camera.GetForward();
			return UnitVector3f(cameraForward.X(), 0, cameraForward.Z());
		}();

		{
			const Ray3f ray = currentScene.GetMouseRay();

			struct RayHitResult
			{
				Simple::UnitVector3f mNormal;
				Simple::Physics::PhysicsObject* physicsObject = nullptr;
			};
			std::optional<RayHitResult> hitResult;

			for (auto [physicsComponent, ballComponent] : view)
			{
				if (Spheref* sphere = std::get_if<Spheref>(&physicsComponent.physicsObject.GetShape()))
				{
					const auto result = Simple::IntersectionRaySphere(ray, *sphere);

					if (result)
					{
						const Simple::UnitVector3f launchDirection = Simple::GetUnitVector(result->hitPoint, sphere->GetCenter());
						hitResult = RayHitResult{ launchDirection, &physicsComponent.physicsObject };
					}
				}
			}

			if (hitResult)
			{
				const RayHitResult& result = hitResult.value();
				const Simple::UnitVector3f& launchDirection = result.mNormal;
				const UnitVector3f curveDir = Simple::UnitVector3f(launchDirection.X(), 0, launchDirection.Z());

				const Radiansf angle = Simple::CalculateAngle(launchDirection, curveDir);

				const UnitVector3f launchDir = RotateVectorAroundAxis(cameraForward, Cross(cameraForward, UnitVector3f::Up()), angle);

				Simple::Physics::Momentum3f momentum(launchDir * playerComponent.shotPower);
				result.physicsObject->SetVelocity(Simple::Physics::ApplyImpulseForce(result.physicsObject->GetVelocity(), momentum, result.physicsObject->GetMass()));

				const Radiansf a = CalculateSignedAngle(curveDir, cameraForward, UnitVector3f::Up());
				result.physicsObject->SetCurve(Square(a.Value() * 2.5f) * Sign(a.Value()));
			}
		}
}

void BallSystem::Update(Simple::ECS& ecs, const float deltaTime, const Simple::Blackboard& blackboard)
{
	UpdateBalls(ecs, deltaTime, blackboard.Get<Key_CurrentCamera>(), blackboard.Get<Key_InputState>(), blackboard.Get<Key_SceneManager>().GetCurrentScene());

}