#pragma once
#include "Graphics/GraphicsConstants.hpp"
#include "Utility/Asset/AnimationAsset.hpp"
#include "Graphics/Model/Bone.hpp"
#include <span>

namespace Simple
{

	class AnimationPlayer final
	{
	public:

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