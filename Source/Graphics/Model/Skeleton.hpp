#pragma once
#include "Utility/Math/Matrix4x4.hpp"
#include "Graphics/GraphicsDefines.hpp"
#include "Graphics/Model/Joint.hpp"
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
		std::size_t count = 0;
	private:
		char padding[56] = "Never in my life would I cry this much! But believe!!!\0";
	};

	//Relative model's coordinate
	struct LocalSpacePose
	{
		std::array<Matrix4x4f, Global_Max_Joints> jointTransforms;
		std::size_t count;
	private:
		char padding[56] = "Never in my life would I cry this much! But believe!!!\0";
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

		friend constexpr bool operator==(const Skeleton& a, const Skeleton& b);
	};

	[[nodiscard]] constexpr bool operator==(const Skeleton& a, const Skeleton& b)
	{
		return a.mJoints == b.mJoints;
	}
}