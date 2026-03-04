#pragma once
#include <limits>

namespace CLX
{

	class RenderTargetView final
	{
	public:

		constexpr RenderTargetView() = default;

		template<typename T>
		constexpr RenderTargetView(T& renderTarget, unsigned int id)
			: mPtr(&renderTarget)
			, mID(id)
			, mGetSRV([](void* ptr) -> void* { return GetRenderTargetSRV(*static_cast<T*>(ptr)); })
		{
		}

		[[nodiscard]] constexpr unsigned int GetID() const noexcept
		{
			return mID;
		}

		[[nodiscard]] constexpr void* GetSRV() const noexcept
		{
			return mGetSRV(mPtr);
		}

	private:

		void* mPtr = nullptr;
		unsigned int mID = std::numeric_limits<unsigned int>::max();
		void* (*mGetSRV)(void*) = nullptr;
	};
}