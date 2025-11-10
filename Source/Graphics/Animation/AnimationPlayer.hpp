#pragma once
#include "Graphics/GraphicsConstants.hpp"
#include "Utility/Asset/AnimationAsset.hpp"

namespace Simple
{

	class AnimationPlayer final
	{
	public:

		void Update(const float deltaTime);

		std::array<Matrix4x4f, GlobalMaxBones> localBoneMatrices;


		void SetAnimation(const AnimationAssetHandle& animationAsset)
		{
			mAnimationAsset = animationAsset;
			mCurrentTime = 0.f;
		}
		
		AnimationAssetHandle mAnimationAsset;

	private:


		float mCurrentTime = 0.f;

	};
}