#pragma once
#include <utility>

namespace CLX
{
	template<typename Allocator, typename Deallocator>
	class FunctionResource final
	{
	public:

		template<typename... Args>
		constexpr FunctionResource(Allocator allocator = Allocator{}, Deallocator deallocator = Deallocator{}, Args&&... args)
			: mAllocator(allocator)
			, mDeallocator(deallocator)
            , mBool(true)
		{
            mAllocator(std::forward<Args>(args)...);
		}

		constexpr ~FunctionResource()
		{
			if (mBool)
			{
				mDeallocator();
			}
		}

		FunctionResource(const FunctionResource&) = delete;
		FunctionResource& operator=(const FunctionResource&) = delete;

		FunctionResource(FunctionResource&& other) noexcept
			: mAllocator(std::move(other.mAllocator))
			, mDeallocator(std::move(other.mDeallocator))
			, mBool(std::exchange(other.mBool, false))
		{
		}

		FunctionResource& operator=(FunctionResource&& other) noexcept
		{
			mAllocator = std::move(other.mAllocator);
			mDeallocator = std::move(other.mDeallocator);
			mBool = std::exchange(other.mBool, false);

			return *this;
		}

	private:
		[[no_unique_address]] Allocator mAllocator = Allocator{};
		[[no_unique_address]] Deallocator mDeallocator = Deallocator{};
        bool mBool = false;
	};
}