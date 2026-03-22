#include "Engine/Precompiled/EnginePch.hpp"
#include "AnimationPlayer.hpp"
#include "Engine/Graphics/Animation/Animation.hpp"
#include <span>
#include "Engine/Graphics/Model/Bone.hpp"

namespace CLX
{

	std::pair<std::size_t, std::size_t> FindKeyFrameIndexPair(const std::vector<float>& timestamps, float time)
	{
		size_t i = 0;
		while (i + 1 < timestamps.size() && timestamps[i + 1] < time)
		{
			i++;
		}

		size_t next = i + 1 < timestamps.size() ? i + 1 : i;

		return { i, next };
	}

	constexpr float GetTimeFactor(const std::vector<float>& timestamps, const float time, std::size_t from, std::size_t to)
	{
		const float t0 = timestamps[from];
		const float t1 = timestamps[to];

		return Remap0To1(time, t0, t1);//(time - t0) / (t1 - t0);

	}

	Matrix4x4f SampleBoneTransform(const float time, const BoneKeyFrames& keyframes, const Bone& bone)
	{
		if (true)
		{
			return bone.localBindPose;
		}

		if (keyframes.positions.empty() &&
			keyframes.rotations.empty() &&
			keyframes.scales.empty())
		{
			return bone.localBindPose;
		}

		

		Point3f t;
		{
			if (keyframes.positions.empty())
			{
				t = bone.localBindPose.GetTranslation();
			}
			else
			{
				auto timeIndexPair = FindKeyFrameIndexPair(keyframes.positionTimestamps, time);
				if (timeIndexPair.first == timeIndexPair.second)
				{
					t = keyframes.positions[timeIndexPair.first];
				}
				else
				{
					const float positonFactor = GetTimeFactor(keyframes.positionTimestamps, time, timeIndexPair.first, timeIndexPair.second);

					const Point3f positionFrom = keyframes.positions[timeIndexPair.first];
					const Point3f positionTo = keyframes.positions[timeIndexPair.second];

					t = Lerp(positionFrom, positionTo, positonFactor);
				}
			}
		}

		Quaternionf r;
		{
			if (keyframes.rotations.empty())
			{
				r = ToQuaternion(bone.localBindPose);
			}
			else
			{
				auto timeIndexPair = FindKeyFrameIndexPair(keyframes.rotationTimestamps, time);
				if (timeIndexPair.first == timeIndexPair.second)
				{
					r = keyframes.rotations[timeIndexPair.first];
				}
				else
				{
					const float rotationFactor = GetTimeFactor(keyframes.rotationTimestamps, time, timeIndexPair.first, timeIndexPair.second);
					const Quaternionf rotationFrom = keyframes.rotations[timeIndexPair.first];
					const Quaternionf rotationTo = keyframes.rotations[timeIndexPair.second];
					r = Slerp(rotationFrom, rotationTo, rotationFactor);
				}
			}
		}

		Vector3f s;
		{
			if (keyframes.scales.empty())
			{
				s = bone.localBindPose.GetScale();
			}
			else
			{
				auto timeIndexPair = FindKeyFrameIndexPair(keyframes.scaleTimestamps, time);
				if (timeIndexPair.first == timeIndexPair.second)
				{
					s = keyframes.scales[timeIndexPair.first];
				}
				else
				{
					const float scaleFactor = GetTimeFactor(keyframes.scaleTimestamps, time, timeIndexPair.first, timeIndexPair.second);

					const Vector3f scaleFrom = keyframes.scales[timeIndexPair.first];
					const Vector3f scaleTo = keyframes.scales[timeIndexPair.second];
					s = Lerp(scaleFrom, scaleTo, scaleFactor);
				}
			}
		}

		// 1) Find keyframe interval (t0 -> t1)


		return Matrix4x4f::CreateTRSMatrix(t, ToMatrix(r), s);
	}

	void AnimationPlayer::UpdateTime(const float deltaTime)
	{
		if (!mAnimationAsset.IsValid())
		{
			return;
		}
		const Animation& animation = mAnimationAsset.Get();
		mCurrentTime += deltaTime;

		if (mCurrentTime > animation.duration)
		{
			mCurrentTime = fmod(mCurrentTime, animation.duration);
		}
	}

	void AnimationPlayer::UpdateAnimation(std::span<const Bone> skeletonBones)
	{
		UpdateAnimation(mCurrentTime, skeletonBones);
	}

	void AnimationPlayer::UpdateAnimation(const float currentTime, std::span<const Bone> skeletonBones)
	{
		if (!mAnimationAsset.IsValid())
		{
			return;
		}

		const Animation& animation = mAnimationAsset.Get();

		for (std::size_t boneIndex = 0; boneIndex < animation.boneKeyFrames.size(); boneIndex++)
		{
			localBoneMatrices[boneIndex] = SampleBoneTransform(currentTime, animation.boneKeyFrames[boneIndex], skeletonBones[boneIndex]);
		}
	}

	void AnimationPlayer::Update(const float deltaTime, std::span<const Bone> skeletonBones)
	{
		if (!mAnimationAsset.IsValid())
		{
			return;
		}

		UpdateTime(deltaTime);
		UpdateAnimation(skeletonBones);
	}

}