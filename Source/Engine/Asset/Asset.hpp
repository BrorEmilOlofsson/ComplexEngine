#pragma once
#include <memory>
#include <utility>
#include <filesystem>

namespace CLX
{

	template<typename T>
	struct AssetData
	{
		T object;
        std::filesystem::path relativePath;
	};

	template<typename T>
	class Asset final
	{
	public:

		using AssetType = T;

		constexpr Asset() = default;

		constexpr explicit Asset(T asset, std::filesystem::path relativePath)
			: mAssetData(std::make_shared<AssetData<T>>(AssetData<T>{std::move(asset), std::move(relativePath)}))
		{
		}

		[[nodiscard]] constexpr std::shared_ptr<AssetData<T>> Get()
		{
			return mAssetData;
		}

		[[nodiscard]] constexpr std::shared_ptr<const AssetData<T>> Get() const
		{
			return mAssetData;
		}

		[[nodiscard]] constexpr const std::filesystem::path& GetRelativePath() const
		{
			return mAssetData->relativePath;
        }

		constexpr void SetRelativePath(std::filesystem::path relativePath)
		{
			mAssetData->relativePath = std::move(relativePath);
        }

		[[nodiscard]] constexpr T* operator->() noexcept
		{
			return &mAssetData->object;
		}

		[[nodiscard]] constexpr const T* operator->() const noexcept
		{
			return &mAssetData->object;
		}

		[[nodiscard]] constexpr operator bool() const
		{
			return mAssetData != nullptr;
		}

		[[nodiscard]] static constexpr Asset<T> Empty()
		{
			return Asset<T>();
		}

	private:

		std::shared_ptr<AssetData<T>> mAssetData;
	};

}