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

		constexpr explicit AssetHandle(std::weak_ptr<AssetData<T>> asset)
			: mAssetData(std::move(asset))
		{
		}

		constexpr explicit AssetHandle(Asset<T> asset)
			: mAssetData(std::move(asset.Get()))
		{
		}

		[[nodiscard]] T& Get() noexcept
		{
			return mAssetData.lock()->object;
		}

		[[nodiscard]] const T& Get() const noexcept
		{
			return mAssetData.lock()->object;
		}

		[[nodiscard]] constexpr const std::filesystem::path& GetRelativePath() const noexcept
		{
			return mAssetData.lock()->relativePath;
        }

		[[nodiscard]] constexpr bool IsValid() const noexcept
		{
			return !mAssetData.expired();
		}

		[[nodiscard]] constexpr explicit operator bool() const noexcept
		{
			return IsValid();
		}

		[[nodiscard]] T* operator->() const
		{
			return &mAssetData.lock()->object;
		}

		[[nodiscard]] static constexpr AssetHandle<T> Empty()
		{
			return AssetHandle<T>();
        }

	private:

		std::weak_ptr<AssetData<T>> mAssetData;
	};

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const AssetHandle<T>& a, const AssetHandle<T>& b) noexcept
	{
		if (!a.IsValid() || !b.IsValid())
		{
			return false;
		}

		return &a.Get() == &b.Get();
	}

}