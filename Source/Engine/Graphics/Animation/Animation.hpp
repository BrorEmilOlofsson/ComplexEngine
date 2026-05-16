#pragma once
#include "Engine/Math/Matrix4x4.hpp"
#include <vector>
#include <string>

namespace CLX
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

		float duration = 0.0f;
		float framesPerSecond = 0.0f;
	};
}