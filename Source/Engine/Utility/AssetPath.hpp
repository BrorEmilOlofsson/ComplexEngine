#pragma once
#include <filesystem>
#include "Engine/Utility/StrongType.hpp"
#include "Engine/Utility/FixedString.hpp"

namespace CLX
{
	template<typename Tag>
	using AssetPath = StrongType<FixedString<256>, Tag>;

	using AssetPath_OBJ = AssetPath<struct Tag_OBJ>;
	
}