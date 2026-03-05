#pragma once
#include "Engine/Graphics/GraphicsConstants.hpp"
#include "Engine/Asset/AssetTypes/AnimationAsset.hpp"
#include "Engine/Graphics/Model/Bone.hpp"
#include <span>

namespace CLX
{

	class AnimationPlayer final
	{
	public:

		void UpdateTime(const float deltaTime);
		void UpdateAnimation(std::span<const Bone> skeletonBones);
		void UpdateAnimation(const float currentTime, std::span<const Bone> skeletonBones);
		void Update(const float deltaTime, std::span<const Bone> skeletonBones);

		std::array<Matrix4x4f, GlobalMaxBones> localBoneMatrices;


		void SetAnimation(const AnimationAssetHandle& animationAsset)
		{
			mAnimationAsset = animationAsset;
			mCurrentTime = 0.f;
		}
		
		AnimationAssetHandle mAnimationAsset;
		float mCurrentTime = 0.f;

	private:



	};
}