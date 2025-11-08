#include "Engine/Precompiled/EnginePch.hpp"
#include "AnimationSystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/AnimationComponent.hpp"
#include "Graphics/Model/Bone.hpp"

namespace Simple
{

	void CalculateAnimationPose(std::span<const Bone> bones, std::span<const Matrix4x4f> inMatrices, std::span<Matrix4x4f> outMatrices)
	{
		assert(bones.size() == inMatrices.size());
		assert(bones.size() == outMatrices.size());
		std::vector<Matrix4x4f> localTransforms(bones.size());
		std::vector<Matrix4x4f> modelTransforms(bones.size());

		for (std::size_t i = 0; i < bones.size(); ++i)
		{
			localTransforms[i] = bones[i].localMatrix * inMatrices[i];
		}

		modelTransforms[0] = localTransforms[0];

		for (std::size_t i = 1; i < bones.size(); ++i)
		{
			const uint32_t parentIndex = bones[i].parentIndex;
			modelTransforms[i] = modelTransforms[parentIndex] * localTransforms[i];
		}

		for (std::size_t i = 0; i < bones.size(); ++i)
		{
			outMatrices[i] = modelTransforms[i] * bones[i].inverseModelMatrix;
		}
	}

	void AnimationSystem::Update(ECS& ecs, const float, const Blackboard&)
	{
		ecs.ForEach([](AnimationComponent& animationComponent)
			{
				if (!animationComponent.animation)
				{
					return;
				}
				//animationComponent.animationPlayer.UpdateTest(&animationComponent, deltaTime);
			});
	}
}