#pragma once
#include <string_view>
#include <memory>
#include "Utility/Asset/Asset.hpp"

namespace Simple
{

	template<typename T>
	class AssetHandle final
	{
	public:

		constexpr AssetHandle() = default;

		constexpr explicit AssetHandle(std::weak_ptr<T> asset)
			: mAsset(std::move(asset))
		{
		}

		constexpr explicit AssetHandle(Asset<T> asset)
			: mAsset(std::move(asset.Get()))
		{
		}

		[[nodiscard]] std::shared_ptr<T> Get() noexcept
		{
			return mAsset.lock();
		}

		[[nodiscard]] std::shared_ptr<const T> Get() const noexcept
		{
			return mAsset.lock();
		}

		[[nodiscard]] constexpr bool IsValid() const noexcept
		{
			return !mAsset.expired();
		}

		[[nodiscard]] constexpr operator bool() const noexcept
		{
			return IsValid();
		}

		[[nodiscard]] T* operator->() const
		{
			return mAsset.lock().get();
		}

	private:

		std::weak_ptr<T> mAsset;
	};

}