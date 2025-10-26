#include "Engine/Precompiled/EnginePch.hpp"
#include "CameraSystem.hpp"
#include "Engine/ECSEngine/Components/CameraComponent.hpp"
#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Utility/Win/WinConsole.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/Camera/CameraMovement.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformUtility.hpp"
#include "Utility/Input/InputState.hpp"
#include "Graphics/RenderState.hpp"
#include "Engine/Utility/DebugShapes.hpp"

namespace Simple
{

	static void SetCamera(ECS& ecs, RenderState& renderState)
	{

		auto cameraComponentView = ecs.ViewUsingEntityID<CameraComponent, TransformComponent>();

		if (cameraComponentView.IsEmpty())
		{
			//renderState.SetCamera(aGraphicsEngine.GetDefaultCamera());
			Console::Print("No Camera found in current scene", ConsoleTextColor::Red);
			return;
		}

		auto [entityID, cameraComponent, transformComponent] = *cameraComponentView.begin();

		Transform worldTransform = GetWorldTransform(ecs, entityID);
		cameraComponent.camera.SetPosition(worldTransform.GetPosition());
		cameraComponent.camera.SetRotation(worldTransform.GetRotation());
		renderState.SetCamera(cameraComponent.camera);
	}

	static void PushCameraLines(const ECS& ecs, RenderList& renderList)
	{
		auto cameraComponentView = ecs.View<CameraComponent, TransformComponent>();

		if (!cameraComponentView.IsEmpty())
		{
			auto [cameraComponent, transformComponent] = *cameraComponentView.begin();

			const UnitVector3f forward = cameraComponent.camera.GetForward();
			
			const Camera& camera = cameraComponent.camera;

			Frustrumf frustrum;
			frustrum.mFarPlaneV = camera.GetFarPlane();
			frustrum.mNearPlaneV = camera.GetNearPlane();
			frustrum.mTransform = camera.GetTransform();
			frustrum.mVerticalAngle = ToRadians(Degreesf(30.f));
			frustrum.mHorizontalAngle = camera.GetHorizontalFOV();
			RenderFrustrum(frustrum, Colors::Lime, renderList);
			DrawLine line;
			line.color = Colors::Red;
			line.startPosition = transformComponent.transform.GetPosition();
			line.endPosition = line.startPosition + forward;

			DrawSphere sphere;
			sphere.sphere = Spheref(line.endPosition, 0.1f);
			sphere.color = Colors::Red;

			DrawSphere sphere2;
			sphere2.sphere = Spheref(line.startPosition, 0.25f);
			sphere2.color = Colors::Gray;

			renderList.AddSphere(sphere, false);
			renderList.AddSphere(sphere2, false);
			renderList.AddLine(line);
		}
	}

	static void HandleFreeFly(ECS& ecs, const float deltaTime, const InputState& input)
	{
		if (!input.IsKeyHeld(eInputKey::RMB))
		{
			return;
		}

		ecs.ForEach([&](CameraComponent& cameraComponent, TransformComponent& transformComponent)
			{
				if (cameraComponent.freeFly)
				{
					FreeFlyCameraUpdate(cameraComponent.camera, deltaTime, 5.f, 45.f, input, CameraControls{});
					transformComponent.transform = cameraComponent.camera.GetTransform();
				}
			});
	}

	void CameraSystem::Update(ECS& ecs, const float deltaTime, const Blackboard& blackboard)
	{
		SetCamera(ecs, blackboard.Get<Key_CurrentRenderState>());
		HandleFreeFly(ecs, deltaTime, blackboard.Get<Key_InputState>());
	}

	void CameraSystem::Render(const ECS& ecs, const Blackboard& blackboard)
	{
		if (blackboard.Get<Key_IsPlaying>())
		{
			return;
		}
		PushCameraLines(ecs, blackboard.Get<Key_CurrentRenderState>().GetRenderList());
	}
}