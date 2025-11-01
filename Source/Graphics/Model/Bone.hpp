#pragma once
#include <limits>
#include <cstdint>
#include "Utility/Math/Matrix4x4.hpp"

namespace Simple
{


	struct Bone final
	{
		Matrix4x4f localMatrix;
		Matrix4x4f inverseModelMatrix;
		uint32_t parentID = std::numeric_limits<uint32_t>::max();
	};

}