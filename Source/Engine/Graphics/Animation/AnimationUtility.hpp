#pragma once
#include "Engine/Graphics/Model/Bone.hpp"
#include "Engine/Graphics/GraphicsConstants.hpp"
#include "Engine/Math/RotationMath.hpp"
#include <span>

namespace CLX
{
	constexpr std::array<Matrix4x4f, GlobalMaxBones> ComputeGlobalTransforms(
		std::span<const Matrix4x4f> localTransforms, std::span<const Bone> bones)
	{
		assert(localTransforms.size() == bones.size());
		assert(bones.size() <= GlobalMaxBones);
		std::array<Matrix4x4f, GlobalMaxBones> globalTransforms{};

		for (uint32_t i = 0; i < bones.size(); ++i)
		{
			const Bone& bone = bones[i];


			if (bone.parentIndex == std::numeric_limits<uint32_t>::max())
			{
				globalTransforms[i] = localTransforms[i];
			}
			else
			{
				globalTransforms[i] = ToWorldSpace(localTransforms[i], globalTransforms[bone.parentIndex]);
			}
		}

		return globalTransforms;
	}

	constexpr std::array<Matrix4x4f, GlobalMaxBones> CalculateFinalBoneMatrices(std::span<const Matrix4x4f> globalMatrices, std::span<const Bone> bones)
	{
		std::array<Matrix4x4f, GlobalMaxBones> finalBoneMatrices{};

		for (size_t i = 0; i < bones.size(); i++)
		{
			finalBoneMatrices[i] = bones[i].inverseBindPose * globalMatrices[i];
		}

		return finalBoneMatrices;
	}
}