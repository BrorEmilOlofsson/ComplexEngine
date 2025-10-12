#pragma once
#include <limits>

#undef max

namespace Simple
{

	struct EntityID final
	{
		[[nodiscard]] constexpr explicit operator unsigned int() const
		{
			return id;
		}

		[[nodiscard]] friend constexpr bool operator==(const EntityID& a, const EntityID& b)
		{
			return a.id == b.id;
		}

		[[nodiscard]] friend constexpr bool operator<(const EntityID& a, const EntityID& b)
		{
			return a.id < b.id;
		}


		unsigned int id = std::numeric_limits<unsigned int>::max();
	};

	constexpr EntityID InvalidEntityID = EntityID{};
}