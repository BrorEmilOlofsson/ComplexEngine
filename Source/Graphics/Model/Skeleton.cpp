#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "Graphics/Model/Skeleton.hpp"
#include <span>

namespace Simple
{


	static void ConvertToLocalSpacePose(std::span<const Joint> joints, const ModelSpacePose& inPose, LocalSpacePose& outPose, unsigned boneID, const Matrix4x4f& parentTransform)
	{
		const Joint& joint = joints[boneID];

		outPose.jointTransforms[boneID] = inPose.jointTransforms[boneID] * parentTransform;

		for (size_t childIndex = 0; childIndex < joint.mChildren.size(); childIndex++)
		{
			ConvertToLocalSpacePose(joints, inPose, outPose, joint.mChildren[childIndex], outPose.jointTransforms[boneID]);
		}
	}

	const Joint* Skeleton::GetRoot() const
	{
		if (mJoints.empty())
		{
			return nullptr;
		}

		return &mJoints[0];
	}

	const std::filesystem::path& Skeleton::GetRelativePath() const
	{
		return mRelativePath;
	}

	void Skeleton::ConvertModelSpacePoseToLocalSpacePose(const ModelSpacePose& inPose, LocalSpacePose& outPose) const
	{
		ConvertToLocalSpacePose(mJoints, inPose, outPose, 0, Matrix4x4f::Identity());
		outPose.count = inPose.count;
	}

	std::array<Matrix4x4f, Global_Max_Joints> Skeleton::ApplyBindPoseInverse(const LocalSpacePose& inPose) const
	{
		std::array<Matrix4x4f, Global_Max_Joints> outMatrix;
		for (size_t i = 0; i < mJoints.size(); ++i)
		{
			const Joint& joint = mJoints[i];
			outMatrix[i] = joint.mBindPoseInverse * inPose.jointTransforms[i];
		}
		return outMatrix;
	}
}