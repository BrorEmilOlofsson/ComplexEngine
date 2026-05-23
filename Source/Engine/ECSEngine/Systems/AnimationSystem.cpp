#include "Engine/Precompiled/EnginePch.hpp"
#include "AnimationSystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/AnimatedModelComponent.hpp"
#include "Engine/Graphics/Model/AnimatedModel.hpp"
#include "Engine/Asset/AssetManager.hpp"
#include "Engine/Graphics/Animation/Animation.hpp"
#include "Engine/Graphics/Animation/AnimationUtility.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include <External/imgui/imgui.h>

namespace CLX
{

	void UpdateAnimations(ECS& ecs, const float deltaTime)
	{
		ecs.ForEach([deltaTime](AnimatedModelComponent& animationComponent)
			{
				if (!animationComponent.modelHandle)
				{
					return;
				}

				if (!animationComponent.animationPlayer.mAnimationAsset.IsValid())
				{
					animationComponent.animationPlayer.mAnimationAsset = animationComponent.defaultAnimation;
				}

				animationComponent.animationPlayer.Update(deltaTime, animationComponent.modelHandle->GetSkeleton().GetBones());
			});
	}

	void AnimationSystem::Update(ECS& ecs, const float deltaTime, const Blackboard&)
	{
        UpdateAnimations(ecs, deltaTime);
	}

	void AnimationSystem::EditorUpdate(ECS& ecs, const Blackboard& blackboard)
	{
		const float deltaTime = blackboard.Get<Key_DeltaTime>();
		UpdateAnimations(ecs, deltaTime);
	}

	void AnimationSystem::Render(const ECS&, const Blackboard& blackboard)
	{
		blackboard;
	}
}