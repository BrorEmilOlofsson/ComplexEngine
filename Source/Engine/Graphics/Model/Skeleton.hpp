#pragma once
#include <array>
#include <span>
#include "Engine/Graphics/Model/Bone.hpp"
#include "Engine/Graphics/GraphicsConstants.hpp"

namespace Simple
{

	class Skeleton final
	{
	public:

		Skeleton() = default;
		explicit Skeleton(const std::array<Bone, GlobalMaxBones>& bones);

		[[nodiscard]] Bone* GetRoot();
		[[nodiscard]] const Bone* GetRoot() const;

		[[nodiscard]] std::span<const Bone> GetBones() const
		{
			return mBones;
		}

	public:

		std::array<Bone, GlobalMaxBones> mBones;
	};
}