#pragma once
#include "Utility/Math/Matrix4x4.hpp"

namespace Simple
{
	class Joint final
	{
	public:
		Matrix4x4f mBindPoseInverse;
		std::string mName;
		std::vector<unsigned int> mChildren;
		std::size_t mParent = std::numeric_limits<std::size_t>::max();
	public:

		friend constexpr bool operator==(const Joint& a, const Joint& b) noexcept;
	};

	[[nodiscard]] constexpr bool operator==(const Joint& a, const Joint& b) noexcept
	{
		return (a.mBindPoseInverse == b.mBindPoseInverse &&
			a.mParent == b.mParent &&
			a.mName == b.mName &&
			a.mChildren == b.mChildren);
	}
}