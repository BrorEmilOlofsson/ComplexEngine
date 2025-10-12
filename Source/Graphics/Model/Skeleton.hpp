#pragma once
#include "Utility/Math/Matrix4x4.hpp"
#include "Graphics/GraphicsDefines.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <filesystem>

namespace Simple
{
	//Relative parent's coordinate
	struct ModelSpacePose
	{
		std::array<Matrix4x4f, Global_Max_Joints> jointTransforms;
		size_t count = 0;
	private:
		char padding[56] = "Never in my life would I cry this much! But believe!!!\0";
	};

	//Relative model's coordinate
	struct LocalSpacePose
	{
		std::array<Matrix4x4f, Global_Max_Joints> jointTransforms;
		size_t count;
	private:
		char padding[56] = "Never in my life would I cry this much! But believe!!!\0";
	};

	class Joint final
	{
	public:
		Matrix4x4f mBindPoseInverse;
		std::string mName;
		std::vector<unsigned int> mChildren;
		int mParent = -1;
	public:

		friend bool operator==(const Joint& a, const Joint& b)
		{
			return (a.mBindPoseInverse == b.mBindPoseInverse &&
					a.mParent == b.mParent &&
					a.mName == b.mName &&
				a.mChildren == b.mChildren);
		}
	};

	class Skeleton final
	{
	public:

		std::string mName;
		std::filesystem::path mRelativePath;
		std::unordered_map<std::string, size_t> mJointNameToIndex;
		std::vector<std::string> mJointNames;
		std::vector<Joint> mJoints;

	public:

		Skeleton() = default;

		const Joint* GetRoot() const;
		const std::filesystem::path& GetRelativePath() const;


		void ConvertModelSpacePoseToLocalSpacePose(const ModelSpacePose& inPose, LocalSpacePose& outPose) const;
		std::array<Matrix4x4f, Global_Max_Joints> ApplyBindPoseInverse(const LocalSpacePose& inPose) const;

		friend bool operator==(const Skeleton& a, const Skeleton& b);
	};

	inline bool operator==(const Skeleton& a, const Skeleton& b)
	{
		return a.mJoints == b.mJoints;
	}
}