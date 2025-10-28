#include "Engine/Precompiled/EnginePch.hpp"
#include "AnimationSystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/AnimationComponent.hpp"

namespace Simple
{

	void AnimationSystem::Update(ECS& ecs, const float, const Blackboard&)
	{
		for (AnimationComponent& animationComponent : ecs.View<AnimationComponent>())
		{
			if (!animationComponent.animation)
			{
				continue;
			}

			//animationComponent.animationPlayer.UpdateTest(&animationComponent, deltaTime);
		}
	}
}