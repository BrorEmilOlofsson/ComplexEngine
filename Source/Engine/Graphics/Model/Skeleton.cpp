#include "Engine/Precompiled/EnginePch.hpp"
#include "Skeleton.hpp"

namespace Simple
{

	Skeleton::Skeleton(const std::array<Bone, GlobalMaxBones>& bones)
		: mBones(bones)
	{
	}

	Bone* Skeleton::GetRoot()
	{
		if (mBones.empty())
		{
			return nullptr;
		}

		return &mBones.front();
	}

	const Bone* Skeleton::GetRoot() const
	{
		if (mBones.empty())
		{
			return nullptr;
		}

		return &mBones.front();
	}
}