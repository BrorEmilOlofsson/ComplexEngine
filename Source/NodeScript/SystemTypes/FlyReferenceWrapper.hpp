#pragma once
#include <type_traits>
#include "../FlyDefines.hpp"

namespace FLY_NAMESPACE
{
	template<typename T>
	class ReferenceWrapper final
	{
		using Ref = T&;
		using Ptr = T*;
		using PtrRef = T*&;
	public:
		static_assert(std::is_object_v<T> || std::is_function_v<T>,
			"ReferenceWrapper<T> requires T to be an object type or a function type.");

		ReferenceWrapper() = default;

		ReferenceWrapper(Ref ref)
			: mPtr(&ref)
		{
		}

		ReferenceWrapper& operator=(T& value)
		{
			Set(value);

			return *this;
		}

		constexpr operator Ref() const noexcept
		{
			return *mPtr;
		}

		[[nodiscard]] constexpr const T& Get() const noexcept
		{
			return *mPtr;
		}

		[[nodiscard]] constexpr T& Get() noexcept
		{
			return *mPtr;
		}

		[[nodiscard]] constexpr T*& GetPtr() noexcept
		{
			return mPtr;
		}

		[[nodiscard]] constexpr const T*& GetPtr() const noexcept
		{
			return mPtr;
		}

		constexpr void Set(Ref value)
		{
			mPtr = &value;
		}

	private:

		Ptr mPtr{};

	public:

		template<typename... Types>
		constexpr auto operator()(Types&&... args) const
			noexcept(noexcept(std::invoke(*mPtr, static_cast<Types&&>(args)...)))
			-> decltype(std::invoke(*mPtr, static_cast<Types&&>(args)...))
		{
			return std::invoke(*mPtr, static_cast<Types&&>(args)...);
		}
	};
}