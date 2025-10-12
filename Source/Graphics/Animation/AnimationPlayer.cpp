#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "Graphics/Animation/AnimationPlayer.hpp"
#include "Engine/ECSEngine/Components/AnimationComponent.hpp"
#include "Graphics/Model/Skeleton.hpp"
#include "Graphics/Animation/Animation.hpp"

namespace Simple
{

	void AnimationPlayer::Init(const Animation* animation, const Skeleton* skeleton)
	{
		mAnimation = animation;
		mSkeleton = skeleton;

		mTime = 0.0f;
	}

	/*void AnimationPlayer::UpdateTest(AnimationComponent* animationPlayerComponent, const float deltaTime)
	{
		if (mState == eAnimationState::Playing)
		{
			mTime += deltaTime;

			if (mTime >= animationPlayerComponent->animation->duration)
			{
				if (mIsLooping)
				{
					while (mTime >= animationPlayerComponent->animation->duration)
					{
						mTime -= animationPlayerComponent->animation->duration;
					}
				}
				else
				{
					mTime = animationPlayerComponent->animation->duration;
					mState = eAnimationState::Finished;
				}
			}

			const float frameRate = 1.0f / animationPlayerComponent->animation->framesPerSecond;
			const float result = mTime / frameRate;
			const std::size_t currentFrame = static_cast<std::size_t>(std::floor(result));
			const float delta = result - static_cast<float>(currentFrame);

			std::size_t nextFrame = currentFrame + 1;

			if (mState == eAnimationState::Finished)
			{
				nextFrame = currentFrame;
			}
			else if (nextFrame > animationPlayerComponent->animation->length)
			{
				nextFrame = 0;
			}

			const Skeleton* skeleton = animationPlayerComponent->skeleton;
			mModelSpacePose.count = skeleton->mJoints.size();

			for (std::size_t i = 0; i < mModelSpacePose.count; i++)
			{
				const Matrix4x4f currentMatrix = animationPlayerComponent->animation->frames[currentFrame].jointNameToModelSpaceMatrix.find(skeleton->mJoints[i].mName)->second;
				const Matrix4x4f nextMatrix = animationPlayerComponent->animation->frames[nextFrame].jointNameToModelSpaceMatrix.find(skeleton->mJoints[i].mName)->second;

				Point3f currentPosition;
				Point3f nextPosition;

				Quaternionf currentQuaternion;
				Quaternionf nextQuaternion;

				Vector3f currentScale;
				Vector3f nextScale;

				currentMatrix.DecomposeMatrix(currentPosition, currentQuaternion, currentScale);
				nextMatrix.DecomposeMatrix(nextPosition, nextQuaternion, nextScale);

				const Point3f translation = LerpClamped(currentPosition, nextPosition, delta);
				const Quaternionf rotation = Quaternionf::Slerp(currentQuaternion, nextQuaternion, delta);
				const Vector3f scale = LerpClamped(currentScale, nextScale, delta);

				const Matrix4x4f lerpedMatrix = Matrix4x4f::CreateScaleMatrix(scale) * rotation.GetRotationMatrix4x4() * Matrix4x4f::CreateTranslationMatrix(translation);
				mModelSpacePose.jointTransforms[i] = lerpedMatrix;
			}

			LocalSpacePose localSpacePose;
			skeleton->ConvertModelSpacePoseToLocalSpacePose(mModelSpacePose, localSpacePose);
			animationPlayerComponent->jointMatrices = skeleton->ApplyBindPoseInverse(localSpacePose);
		}
	}

	void AnimationPlayer::LerpAnimationTest(const std::size_t currentFrame, const std::size_t nextFrame, const float delta)
	{
		const Skeleton* skeleton = mSkeleton;
		mModelSpacePose.count = skeleton->mJoints.size();

		for (std::size_t i = 0; i < mModelSpacePose.count; i++)
		{
			const Matrix4x4f currentMatrix = mAnimation->frames[currentFrame].jointNameToModelSpaceMatrix.find(skeleton->mJoints[i].mName)->second;
			const Matrix4x4f nextMatrix = mAnimation->frames[nextFrame].jointNameToModelSpaceMatrix.find(skeleton->mJoints[i].mName)->second;

			Point3f currentPosition;
			Point3f nextPosition;

			Quaternionf currentQuaternion;
			Quaternionf nextQuaternion;

			Vector3f currentScale;
			Vector3f nextScale;

			currentMatrix.DecomposeMatrix(currentPosition, currentQuaternion, currentScale);
			nextMatrix.DecomposeMatrix(nextPosition, nextQuaternion, nextScale);

			const Point3f translation = LerpClamped(currentPosition, nextPosition, delta);
			const Quaternionf rotation = Quaternionf::Slerp(currentQuaternion, nextQuaternion, delta);
			const Vector3f scale = LerpClamped(currentScale, nextScale, delta);

			const Matrix4x4f lerpedMatrix = Matrix4x4f::CreateScaleMatrix(scale) * rotation.GetRotationMatrix4x4() * Matrix4x4f::CreateTranslationMatrix(translation);
			mModelSpacePose.jointTransforms[i] = lerpedMatrix;
		}
	}*/

	void AnimationPlayer::Play(const bool shouldLoop)
	{
		mState = eAnimationState::Playing;
		SetIsLooping(shouldLoop);
	}

	void AnimationPlayer::Pause()
	{
		mState = eAnimationState::Paused;
	}

	void AnimationPlayer::Stop()
	{
		mState = eAnimationState::Finished;
		mTime = 0.0f;
	}

	void AnimationPlayer::Restart()
	{
		mTime = 0.0f;
		mState = eAnimationState::Playing;
	}

	void AnimationPlayer::SetIsLooping(const bool shouldLoop)
	{
		mIsLooping = shouldLoop;
	}

	void AnimationPlayer::SetCurrentFrame(const unsigned int currentFrame)
	{
		mTime = currentFrame / mAnimation->framesPerSecond;
	}

	const ModelSpacePose AnimationPlayer::GetLocalSpacePose() const
	{
		return mModelSpacePose;
	}

	eAnimationState AnimationPlayer::GetAnimationState() const
	{
		return mState;
	}

	float AnimationPlayer::GetTime() const
	{
		return mTime;
	}

	unsigned int AnimationPlayer::GetCurrentFrame() const
	{
		const float frameRate = 1.0f / mAnimation->framesPerSecond;
		const float result = mTime / frameRate;
		const unsigned int currentFrame = static_cast<unsigned int>(std::floor(result));

		return currentFrame;
	}

	void AnimationPlayer::CalculateFrame(size_t& aCurrentFrame, size_t& aNextFrame, float& aDelta)
	{
		const float frameRate = 1.0f / mAnimation->framesPerSecond;
		const float result = mTime / frameRate;
		const size_t currentFrame = static_cast<size_t>(std::floor(result));
		const float delta = result - static_cast<float>(currentFrame);

		size_t nextFrame = currentFrame + 1;

		if (mState == eAnimationState::Finished)
		{
			nextFrame = currentFrame;
		}
		else if (nextFrame > mAnimation->length)
		{
			nextFrame = 0;
		}

		aCurrentFrame = currentFrame;
		aNextFrame = nextFrame;
		aDelta = delta;
	}

	void AnimationPlayer::UpdateTimer(const float deltaTime)
	{
		mTime += deltaTime;

		if (mTime >= mAnimation->duration)
		{
			if (mIsLooping)
			{
				while (mTime >= mAnimation->duration)
				{
					mTime -= mAnimation->duration;
				}
			}
			else
			{
				mTime = mAnimation->duration;
				mState = eAnimationState::Finished;
			}
		}
	}
}