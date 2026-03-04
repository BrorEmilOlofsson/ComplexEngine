#pragma once
#include <limits>
#include <string>
#include <cstdint>
#include "Engine/Math/Matrix4x4.hpp"

namespace CLX
{

	struct Bone final
	{
		Matrix4x4f inverseBindPose;
		Matrix4x4f localBindPose;
		std::string name;
		uint32_t parentIndex = std::numeric_limits<uint32_t>::max();
	};

}