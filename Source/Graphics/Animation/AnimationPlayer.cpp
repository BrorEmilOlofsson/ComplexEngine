#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "AnimationPlayer.hpp"
#include "Graphics/Animation/Animation.hpp"
#include <span>
#include "Graphics/Model/Bone.hpp"

namespace Simple
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
				auto positionTimeIndexPair = FindKeyFrameIndexPair(keyframes.positionTimestamps, time);
				const float positonFactor = GetTimeFactor(keyframes.positionTimestamps, time, positionTimeIndexPair.first, positionTimeIndexPair.second);

				const Point3f positionFrom = keyframes.positions[positionTimeIndexPair.first];
				const Point3f positionTo = keyframes.positions[positionTimeIndexPair.second];

				t = Lerp(positionFrom, positionTo, positonFactor);
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
				auto rotationTimeIndexPair = FindKeyFrameIndexPair(keyframes.rotationTimestamps, time);
				const float rotationFactor = GetTimeFactor(keyframes.rotationTimestamps, time, rotationTimeIndexPair.first, rotationTimeIndexPair.second);
				const Quaternionf rotationFrom = keyframes.rotations[rotationTimeIndexPair.first];
				const Quaternionf rotationTo = keyframes.rotations[rotationTimeIndexPair.second];
				r = Slerp(rotationFrom, rotationTo, rotationFactor);
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
				auto scaleTimeIndexPair = FindKeyFrameIndexPair(keyframes.scaleTimestamps, time);
				const float scaleFactor = GetTimeFactor(keyframes.scaleTimestamps, time, scaleTimeIndexPair.first, scaleTimeIndexPair.second);


				const Vector3f scaleFrom = keyframes.scales[scaleTimeIndexPair.first];
				const Vector3f scaleTo = keyframes.scales[scaleTimeIndexPair.second];

				s = Lerp(scaleFrom, scaleTo, scaleFactor);
				
			}
		}

		// 1) Find keyframe interval (t0 -> t1)
		

		return Matrix4x4f::CreateTRSMatrix(t, ToMatrix(r), s);
	}


	void AnimationPlayer::Update(const float deltaTime, std::span<const Bone> skeletonBones)
	{
		if (!mAnimationAsset.IsValid())
		{
			return;
		}

		const Animation& animation = *mAnimationAsset.Get();
		mCurrentTime += deltaTime;

		if (mCurrentTime > animation.duration)
		{
			mCurrentTime = fmod(mCurrentTime, animation.duration);
		}

		for (std::size_t boneIndex = 0; boneIndex < animation.boneKeyFrames.size(); boneIndex++)
		{
			localBoneMatrices[boneIndex] = SampleBoneTransform(mCurrentTime, animation.boneKeyFrames[boneIndex], skeletonBones[boneIndex]);
		}
	}

}