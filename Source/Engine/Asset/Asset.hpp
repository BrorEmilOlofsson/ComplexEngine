#pragma once
#include <memory>
#include <utility>
#include <filesystem>

namespace CLX
{

	template<typename T>
	class Asset final
	{
	public:

		constexpr Asset() = default;

		constexpr explicit Asset(std::shared_ptr<T> asset, std::filesystem::path relativePath)
			: mAsset(std::move(asset))
			,  mRelativePath(std::move(relativePath))
		{
		}

		[[nodiscard]] constexpr std::shared_ptr<T> Get()
		{
			return mAsset;
		}

		[[nodiscard]] constexpr std::shared_ptr<const T> Get() const
		{
			return mAsset;
		}

		[[nodiscard]] constexpr const std::filesystem::path& GetRelativePath() const
		{
			return mRelativePath;
        }

		[[nodiscard]] constexpr T* operator->() noexcept
		{
			return mAsset.get();
		}

		[[nodiscard]] constexpr const T* operator->() const noexcept
		{
			return mAsset.get();
		}

		[[nodiscard]] constexpr operator bool() const
		{
			return mAsset != nullptr;
		}

		[[nodiscard]] static constexpr Asset<T> Empty()
		{
			return Asset<T>();
		}

	private:

		std::shared_ptr<T> mAsset;
        std::filesystem::path mRelativePath;
	};

}