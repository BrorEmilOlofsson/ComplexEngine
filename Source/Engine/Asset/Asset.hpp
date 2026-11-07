#pragma once
#include <memory>
#include <utility>
#include <filesystem>
#include <variant>
#include "Engine/Utility/Visitor.hpp"
#include "Engine/Utility/File/FileUtility.hpp"

namespace CLX
{

    using AssetPathID = std::variant<std::filesystem::path, std::string>;

	template<typename T>
	struct AssetData
	{
		T object;
		AssetPathID path;
	};

	constexpr AssetPathID MakeRelativePath(const AssetPathID& path)
	{
        return std::visit(Visitor{
			[](const std::filesystem::path& p) -> AssetPathID { return GetRelativeAssetPath(p); },
			[] (const std::string& p) -> AssetPathID { return p; }
		}, path);
	}

	template<typename T>
	class Asset final
	{
	public:

		using AssetType = T;
		
		constexpr Asset() = default;

		constexpr explicit Asset(T asset, AssetPathID path)
			: mAssetData(std::make_shared<AssetData<T>>(AssetData<T>{std::move(asset), MakeRelativePath(path)}))
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
			return std::get<std::filesystem::path>(mAssetData->path);
        }

		constexpr void SetRelativePath(std::filesystem::path relativePath)
		{
			mAssetData->path = std::move(relativePath);
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