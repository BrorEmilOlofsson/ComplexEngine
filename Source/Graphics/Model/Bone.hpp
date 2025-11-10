#pragma once
#include <limits>
#include <string>
#include <cstdint>
#include "Utility/Math/Matrix4x4.hpp"

namespace Simple
{

	struct Bone final
	{
		Matrix4x4f inverseBindMatrix;
		std::string name;
		uint32_t parentIndex = std::numeric_limits<uint32_t>::max();
	};

}