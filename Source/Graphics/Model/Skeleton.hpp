#pragma once
#include "Graphics/Model/Bone.hpp"
#include <vector>

namespace Simple
{

	class Skeleton final
	{
	public:

		Skeleton() = default;
		Skeleton(const std::vector<Bone>& bones);

		[[nodiscard]] Bone* GetRoot();
		[[nodiscard]] const Bone* GetRoot() const;

	public:

		std::vector<Bone> mBones;
	};
}