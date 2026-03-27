#include "Engine/Precompiled/EnginePch.hpp"
#include "CameraSystem.hpp"
#include "Engine/ECSEngine/Components/CameraComponent.hpp"
#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Engine/Utility/Win/WinConsole.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/Camera/CameraMovement.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformUtility.hpp"
#include "Engine/Input/InputState.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include "Engine/Utility/DebugShapes.hpp"

namespace CLX
{

	static void SetCamera(const ECS& ecs, RenderState& renderState)
	{
        std::vector<const CameraComponent*> cameraComponents;

		ecs.ForEach([&](const CameraComponent& cameraComp)
			{
				cameraComponents.push_back(&cameraComp);
            });

		if (cameraComponents.empty())
		{
			//renderState.SetCamera(aGraphicsEngine.GetDefaultCamera());
			Console::Print("No Camera found in current scene", ConsoleTextColor::Red);
			return;
		}

		auto cameraComponent = cameraComponents.front();

		renderState.SetCamera(cameraComponent->camera);
	}

	static void UpdateCameraTransforms(ECS& ecs)
	{
		ecs.ForEach([&ecs](const EntityID entityID, CameraComponent& cameraComponent)
			{
				cameraComponent.camera.SetTransform(GetWorldTransform(ecs, entityID));
			});
	}

	static void PushCameraLines(const ECS& ecs, RenderList& renderList)
	{
        std::vector<const CameraComponent*> cameraComponents;

        ecs.ForEach([&](const CameraComponent& cameraComp)
			{
                cameraComponents.push_back(&cameraComp);
			});

		if (!cameraComponents.empty())
		{
			auto& cameraComponent = *cameraComponents.front();

			const UnitVector3f forward = cameraComponent.camera.GetForward();
			
			const Camera& camera = cameraComponent.camera;
			Frustrumf frustrum;
			frustrum.mFarPlaneV = camera.GetFarPlane();
			frustrum.mNearPlaneV = camera.GetNearPlane();
			frustrum.mTransform = camera.GetTransform();
			frustrum.mVerticalAngle = camera.GetVerticalFOV();
			frustrum.mHorizontalAngle = camera.GetHorizontalFOV();
			RenderFrustrum(frustrum, Colors::Lime, renderList);

			DrawSphere sphere;
			sphere.sphere = Spheref::FromCenterAndRadius(camera.GetPosition(), Radiusf(0.1f));
			sphere.color = Colors::Gray;

			renderList.AddSphere(sphere, false);
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
		UpdateCameraTransforms(ecs);
		SetCamera(ecs, blackboard.Get<Key_CurrentRenderState>());
		HandleFreeFly(ecs, deltaTime, blackboard.Get<Key_InputState>());
	}

	void CameraSystem::EditorUpdate(ECS& ecs, const Blackboard&)
	{
		UpdateCameraTransforms(ecs);

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