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

		// 1) Find keyframe interval (t0 -> t1)
		auto positionTimeIndexPair = FindKeyFrameIndexPair(keyframes.positionTimestamps, time);
		//auto rotationTimeIndexPair = FindKeyFrameIndexPair(keyframes.rotationTimestamps, time);
		auto scaleTimeIndexPair = FindKeyFrameIndexPair(keyframes.scaleTimestamps, time);

		const float positonFactor = GetTimeFactor(keyframes.positionTimestamps, time, positionTimeIndexPair.first, positionTimeIndexPair.second);
		//const float rotationFactor = GetTimeFactor(keyframes.rotationTimestamps, time, rotationTimeIndexPair.first, rotationTimeIndexPair.second);
		const float scaleFactor = GetTimeFactor(keyframes.scaleTimestamps, time, scaleTimeIndexPair.first, scaleTimeIndexPair.second);

		const Point3f positionFrom = keyframes.positions[positionTimeIndexPair.first];
		const Point3f positionTo = keyframes.positions[positionTimeIndexPair.second];

		//const Quaternionf rotationFrom = keyframes.rotations[rotationTimeIndexPair.first];
		//const Quaternionf rotationTo = keyframes.rotations[rotationTimeIndexPair.second];

		const Vector3f scaleFrom = keyframes.scales[scaleTimeIndexPair.first];
		const Vector3f scaleTo = keyframes.scales[scaleTimeIndexPair.second];

		const Point3f T = Lerp(positionFrom, positionTo, positonFactor);
		//const Quaternionf R = Slerp(rotationFrom, rotationTo, rotationFactor);
		const Vector3f S = Lerp(scaleFrom, scaleTo, scaleFactor);

		const Matrix4x4f R = Matrix4x4f::Identity();

		return Matrix4x4f::CreateTRSMatrix(T, R, S);
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