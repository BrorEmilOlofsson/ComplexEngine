#pragma once
#include <type_traits>

namespace Simple
{
	template<typename Allocator, typename Deallocator>
	class PointerResource final
	{
	public:
		using ResourceType = decltype(Allocator{}());
		static_assert(std::is_pointer_v<ResourceType>);
		constexpr PointerResource(Allocator allocator = Allocator{}, Deallocator deallocator = Deallocator{})
			: mAllocator(allocator)
			, mDeallocator(deallocator)
		{
			mResource = mAllocator();
		}

		constexpr ~PointerResource()
		{
			if (mResource)
			{
				mDeallocator(mResource);
			}
		}

		PointerResource(const PointerResource&) = delete;
		PointerResource& operator=(const PointerResource&) = delete;

		PointerResource(PointerResource&& other) noexcept
			: mAllocator(std::move(other.mAllocator))
			, mDeallocator(std::move(other.mDeallocator))
			, mResource(std::exchange(other.mResource, nullptr))
		{
		}

		PointerResource& operator=(PointerResource&& other) noexcept
		{
			mAllocator = std::move(other.mAllocator);
			mDeallocator = std::move(other.mDeallocator);
			mResource = std::exchange(other.mResource, nullptr);

			return *this;
		}

		[[nodiscard]] ResourceType Get()
		{
			return mResource;
		}

	private:
		[[no_unique_address]] Allocator mAllocator = Allocator{};
		[[no_unique_address]] Deallocator mDeallocator = Deallocator{};
		ResourceType mResource = ResourceType{};
	};
}