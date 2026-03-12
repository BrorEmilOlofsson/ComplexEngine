#pragma once
#include <memory>
#include <utility>
#include <filesystem>
#include "Engine/Asset/Asset.hpp"

namespace CLX
{

	template<typename T>
	class AssetHandle final
	{
	public:

		constexpr AssetHandle() = default;

		constexpr explicit AssetHandle(std::weak_ptr<T> asset, std::filesystem::path relativePath)
			: mAsset(std::move(asset))
			, mRelativePath(std::move(relativePath))
		{
		}

		constexpr explicit AssetHandle(Asset<T> asset)
			: mAsset(std::move(asset.Get()))
			, mRelativePath(std::move(asset.GetRelativePath()))
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

		[[nodiscard]] constexpr const std::filesystem::path& GetRelativePath() const noexcept
		{
			return mRelativePath;
        }

		[[nodiscard]] constexpr bool IsValid() const noexcept
		{
			return !mAsset.expired();
		}

		[[nodiscard]] constexpr explicit operator bool() const noexcept
		{
			return IsValid();
		}

		[[nodiscard]] T* operator->() const
		{
			return mAsset.lock().get();
		}

		[[nodiscard]] static constexpr AssetHandle<T> Empty()
		{
			return AssetHandle<T>();
        }

	private:

		std::weak_ptr<T> mAsset;
		std::filesystem::path mRelativePath;
	};

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const AssetHandle<T>& a, const AssetHandle<T>& b) noexcept
	{
		if (!a.IsValid() || !b.IsValid())
		{
			return false;
		}

		return a.Get().get() == b.Get().get();
	}

}