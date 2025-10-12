#include "Game/Precompiled/GamePch.hpp"
#include "PhysicsSystem.hpp"
#include "Game/ECS/Components/PhysicsComponent.hpp"
#include "Engine/Physics/Physics.hpp"
#include "Engine/Utility/DebugShapes.hpp"
#include "Utility/Math/GeometryMath.hpp"
#include "Engine/Physics/PhysicsCollision.hpp"
#include "Engine/ECSEngine/Components/AllEngineComponents.hpp"
#include "Graphics/RenderState.hpp"

constexpr Simple::Point3f startPoint = Simple::Point3f(-5, 2, -6);

static void UpdateVelocity(Simple::Physics::PhysicsObject& obj, Simple::Transform& aTransform, Simple::Physics::Accelerationf aGravity, const float deltaTime)
{
	if (obj.GetType() == Simple::Physics::ePhysicsObjectType::Static)
	{
		return;
	}
	obj.AddVelocity(Simple::Physics::Velocityf(aGravity.Value() * deltaTime));

	{
		const Simple::Point3f newPos = aTransform.GetPosition() + obj.GetVelocity().Value() * deltaTime;
		obj.SetPosition(newPos);
		aTransform.SetPosition(newPos);
	}

	if (Simple::Spheref* sphere = std::get_if<Simple::Spheref>(&obj.GetShape()))
	{
		constexpr float groundLevel = 0;
		constexpr float dampingFactor = 0.8f;
		if (sphere->GetCenter().y - sphere->GetRadius() < groundLevel)
		{
			// Reverse the y-component of the velocity and apply damping
			obj.SetVelocity(Simple::Physics::Velocityf(Simple::Vector3f(obj.GetVelocity().Value().x, -obj.GetVelocity().Value().y * dampingFactor, obj.GetVelocity().Value().z)));
			obj.SetCurve(obj.GetCurve() * dampingFactor);

			const Simple::Point3f newPos(aTransform.GetPosition().x, groundLevel + sphere->GetRadius(), aTransform.GetPosition().z);
			// Correct the position to avoid sinking into the ground
			sphere->SetCenter(newPos);
			aTransform.SetPosition(newPos);
		}
	}
}


static void ResolveCollision(Simple::Physics::PhysicsObject& aPhysicsObject1, Simple::Physics::PhysicsObject& aPhysicsObject2)
{
	Simple::Physics::PhysicsObjectData data1 = { aPhysicsObject1.GetVelocity(), aPhysicsObject1.GetMass(), aPhysicsObject1.GetType() };
	Simple::Physics::PhysicsObjectData data2 = { aPhysicsObject2.GetVelocity(), aPhysicsObject2.GetMass(), aPhysicsObject2.GetType() };


	const bool didCollide = std::visit(
		[&data1, &data2](auto& shape1, auto& shape2)
		{
			if (!DetectCollision(shape1, shape2))
			{
				return false;
			}
			Simple::Physics::ResolveCollision(shape1, shape2, data1, data2);
			return true;
		},
		aPhysicsObject1.GetShape(),
		aPhysicsObject2.GetShape());

	if (didCollide)
	{
		aPhysicsObject1.SetVelocity(data1.myVelocity);
		aPhysicsObject2.SetVelocity(data2.myVelocity);
		aPhysicsObject1.SetCurve(0);
		aPhysicsObject2.SetCurve(0);
	}

}

static void PushDebugLines([[maybe_unused]] Simple::ECS& ecs, Simple::RenderList& renderList)
{

	constexpr Simple::Planef plane;
	Simple::RenderPlane(plane, Simple::Colors::Green, renderList);


	struct RenderVisitor
	{
		Simple::Color myColor = Simple::Colors::Black;
		Simple::RenderList& myRenderList;

		void operator()(const Simple::Spheref& sphere)
		{
			Simple::RenderSphere(sphere, myColor, myRenderList);
		}
		void operator()(const Simple::Cylinderf& cylinder)
		{
			Simple::RenderCylinder(cylinder, myColor, myRenderList);
		}
	};


	/*for (auto& physicsComponent : ecs.View<PhysicsComponent>())
	{

		std::visit(RenderVisitor{ physicsComponent.myPhysicsObject.GetColor() }, physicsComponent.myPhysicsObject.GetShape());

		if (Simple::Spheref* sphere = std::get_if<Simple::Spheref>(&physicsComponent.myPhysicsObject.GetShape()))
		{
			const auto result = Simple::IntersectionSphereRay(*sphere, Simple::GetMouseRay());

			if (result)
			{
				Simple::RenderSphere(Simple::Spheref(result.value(), 0.02f), Simple::Colors::Yellow);
			}
		}

	}*/
}

void PhysicsSystem::Update(Simple::ECS& ecs, const float deltaTime, const Simple::Blackboard& blackboard)
{
	const Simple::InputState& input = blackboard.Get<Simple::Key_InputState>();
	static bool slowMotion = false;
	if (input.IsKeyPressed(Simple::eInputKey::E))
	{
		slowMotion = !slowMotion;
	}

	const float adjustedDeltaTime = deltaTime * (slowMotion ? 0.05f : 1.f);
	static bool pause = false;
	static bool advanceFrame = false;

	if (input.IsKeyPressed(Simple::eInputKey::P))
	{
		pause = !pause;
	}

	if (input.IsKeyPressed(Simple::eInputKey::F))
	{
		advanceFrame = true;
	}

	if (pause)
	{
		if (!advanceFrame)
		{
			return;
		}

		advanceFrame = false;
	}

	auto view = ecs.View<PhysicsComponent>();

	for (auto& physicsComponent1 : view)
	{
		for (auto& physicsComponent2 : view)
		{
			if (&physicsComponent1 == &physicsComponent2)
			{
				continue;
			}

			ResolveCollision(physicsComponent1.physicsObject, physicsComponent2.physicsObject);

		}
	}

	static Simple::Physics::Accelerationf gravity = Simple::Physics::EARTH_GRAVITY_ACCELERATION<float>;

	for (auto [transformComponent, physicsComponent] : ecs.View<Simple::TransformComponent, PhysicsComponent>())
	{
		UpdateVelocity(physicsComponent.physicsObject, transformComponent.transform, gravity, adjustedDeltaTime);
	}


	if (input.IsKeyPressed(Simple::eInputKey::Q))
	{
		gravity = -gravity;
	}

	Simple::RenderList& renderList = blackboard.Get<Simple::Key_SceneRenderState>().GetRenderList();
	PushDebugLines(ecs, renderList);
}

void PhysicsSystem::EditorUpdate(Simple::ECS& ecs, const Simple::Blackboard& blackboard)
{
	for (auto [physicsComponent, transformComponent] : ecs.View<PhysicsComponent, Simple::TransformComponent>())
	{
		physicsComponent.physicsObject.SetPosition(transformComponent.transform.GetPosition());
	}

	auto view = ecs.View<PhysicsComponent>();
	const Simple::InputState& input = blackboard.Get<Simple::Key_InputState>();

	for (auto& physicsComponent1 : view)
	{
		if (input.IsKeyPressed(Simple::eInputKey::V))
		{
			physicsComponent1.physicsObject.SetColor(Simple::Colors::Blue);
		}
		for (auto& physicsComponent2 : view)
		{
			if (&physicsComponent1 == &physicsComponent2)
			{
				continue;
			}

			const bool collided = std::visit(
				[](const auto& shape1, const auto& shape2)
				{
					return Simple::DetectCollision(shape1, shape2);
				},
				physicsComponent1.physicsObject.GetShape(),
				physicsComponent2.physicsObject.GetShape());

			if (collided)
			{
				physicsComponent1.physicsObject.SetColor(Simple::Colors::Red);
				physicsComponent2.physicsObject.SetColor(Simple::Colors::Red);
			}
		}
	}


	PushDebugLines(ecs, blackboard.Get<Simple::Key_SceneRenderState>().GetRenderList());
}