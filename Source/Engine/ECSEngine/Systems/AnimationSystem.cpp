#include "Engine/Precompiled/EnginePch.hpp"
#include "AnimationSystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/AnimatedModelComponent.hpp"
#include "Graphics/Model/AnimatedModel.hpp"
#include "Utility/Asset/AssetManager.hpp"

namespace Simple
{

	void AnimationSystem::Update(ECS& ecs, const float deltaTime, const Blackboard& blackboard)
	{
		AssetManager& assetManager = blackboard.Get<Key_AssetManager>();
		ecs.ForEach([deltaTime, &assetManager](AnimatedModelComponent& animationComponent)
			{
				if (!animationComponent.modelHandle)
				{
					return;
				}

				if (!animationComponent.animationPlayer.mAnimationAsset.IsValid())
				{
					animationComponent.animationPlayer.mAnimationAsset = assetManager.GetAnimation(std::filesystem::path(SIMPLE_DIR_ASSETS) / "StellaQuest" / "Player" / "Stella_Idle.fbx");
				}

				animationComponent.animationPlayer.Update(deltaTime);
			});
	}

	void AnimationSystem::EditorUpdate(ECS& ecs, const Blackboard& blackboard)
	{
		const float deltaTime = blackboard.Get<Key_DeltaTime>();
		Update(ecs, deltaTime, blackboard);
	}
}