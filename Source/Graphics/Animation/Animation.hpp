#pragma once
#include "Utility/Math/Matrix4x4.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem>

namespace Simple
{
	struct BoneKeyFrames
	{
		std::vector<Point3f> positions;
		std::vector<Quaternionf> rotations;
		std::vector<Vector3f> scales;
		std::vector<float> positionTimestamps;
		std::vector<float> rotationTimestamps;
		std::vector<float> scaleTimestamps;
	};

	class Animation final
	{
	public:
		

		std::vector<BoneKeyFrames> boneKeyFrames;
		std::string name;
		std::filesystem::path path;

		float duration = 0.0f;
		float framesPerSecond = 0.0f;
	};
}