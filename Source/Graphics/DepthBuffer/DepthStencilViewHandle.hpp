#pragma once
#include <limits>

namespace Simple
{

	class DepthStencilViewHandle final
	{
	public:
		
		constexpr DepthStencilViewHandle() = default;

		constexpr DepthStencilViewHandle(unsigned int id)
			: mID(id)
		{
		}

		[[nodiscard]] constexpr unsigned int GetID() const noexcept
		{
			return mID;
		}

	private:

		unsigned int mID = std::numeric_limits<unsigned int>::max();
	};
}