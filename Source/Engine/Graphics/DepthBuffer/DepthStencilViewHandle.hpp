#pragma once
#include <limits>
#include <cstdint>

namespace CLX
{

	class DepthStencilViewHandle final
	{
	public:
		
		constexpr DepthStencilViewHandle() = default;

		constexpr DepthStencilViewHandle(uint32_t id)
			: mID(id)
		{
		}

		[[nodiscard]] constexpr uint32_t GetID() const noexcept
		{
			return mID;
		}

	private:

		uint32_t mID = std::numeric_limits<uint32_t>::max();
	};
}