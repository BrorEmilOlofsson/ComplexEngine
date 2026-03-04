#pragma once
#include <limits>
#include <stdint.h>

namespace CLX
{

	struct EntityID final
	{
		[[nodiscard]] constexpr explicit operator uint32_t() const
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


		uint32_t id = std::numeric_limits<uint32_t>::max();
	};

	constexpr EntityID InvalidEntityID = EntityID{};
}