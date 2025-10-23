#pragma once
#include <vector>
#include <filesystem>
#include <functional>
#include <unordered_map>
#include "Utility/Asset/AssetTypes.hpp"
#include "Utility/Asset/AssetLoader.hpp"

namespace Simple
{

	[[nodiscard]] decltype(auto) MapFind(auto& map, const auto& key)
	{
		auto it = map.find(key);
		if (it != map.end())
		{
			return it->second;
		}
		return decltype(it->second)();
	}



	class AssetManager final
	{
		template<typename HandleType>
		[[nodiscard]] static constexpr decltype(auto) ValidatedGet(auto& path, auto& map)
		{
			if (auto asset = MapFind(map, std::filesystem::absolute(path)))
			{
				return HandleType(asset);
			}
			return HandleType();
		}
	public:

		AssetManager() = default;

		void SetAssetLoader(AssetLoader assetLoader)
		{
			mAssetLoader = std::move(assetLoader);
		}

		void SetDefaultLoader(std::function<void(AssetManager&)> defaultLoader)
		{
			mDefaultLoader = std::move(defaultLoader);
		}

		void LoadAssets();

		[[nodiscard]] TextureAssetHandle GetTexture(const std::filesystem::path& path)
		{
			return ValidatedGet<TextureAssetHandle>(path, mTextureAssets);
		}

		[[nodiscard]] MeshAssetHandle GetMesh(const std::filesystem::path& path)
		{
			return ValidatedGet<MeshAssetHandle>(path, mMeshAssets);
		}

		[[nodiscard]] AnimationAssetHandle GetAnimation(const std::filesystem::path& path)
		{
			return ValidatedGet<AnimationAssetHandle>(path, mAnimationAssets);
		}

		[[nodiscard]] SkeletonAssetHandle GetSkeleton(const std::filesystem::path& path)
		{
			return ValidatedGet<SkeletonAssetHandle>(path, mSkeletonAssets);
		}

		[[nodiscard]] PixelShaderAssetHandle GetPixelShader(const std::filesystem::path& path)
		{
			return ValidatedGet<PixelShaderAssetHandle>(path, mPixelShaderAssets);
		}

		[[nodiscard]] VertexShaderAssetHandle GetVertexShader(const std::filesystem::path& path)
		{
			return ValidatedGet<VertexShaderAssetHandle>(path, mVertexShaderAssets);
		}

		[[nodiscard]] EntityCompositionAssetHandle GetEntityComposition(const std::filesystem::path& path)
		{
			return ValidatedGet<EntityCompositionAssetHandle>(path, mEntityCompositionAssets);
		}

		void AddTexture(const std::filesystem::path& path, TextureAsset asset)
		{
			if (!asset)
			{
				throw std::invalid_argument("Asset is invalid");
			}
			mTextureAssets[std::filesystem::absolute(path)] = std::move(asset);
		}

		void AddMesh(const std::filesystem::path& path, MeshAsset asset)
		{
			if (!asset)
			{
				throw std::invalid_argument("Asset is invalid");
			}
			mMeshAssets[std::filesystem::absolute(path)] = std::move(asset);
		}

		void AddPixelShader(const std::filesystem::path& path, PixelShaderAsset asset)
		{
			if (!asset)
			{
				throw std::invalid_argument("Asset is invalid");
			}
			mPixelShaderAssets[std::filesystem::absolute(path)] = std::move(asset);
		}

		void AddVertexShader(const std::filesystem::path& path, VertexShaderAsset asset)
		{
			if (!asset)
			{
				throw std::invalid_argument("Asset is invalid");
			}
			mVertexShaderAssets[std::filesystem::absolute(path)] = std::move(asset);
		}

		void AddEntityCompositionAsset(const std::filesystem::path& path, EntityCompositionAsset asset)
		{
			if (!asset)
			{
				throw std::invalid_argument("Asset is invalid");
			}
			mEntityCompositionAssets[std::filesystem::absolute(path)] = std::move(asset);
		}

	private:

		std::unordered_map<std::filesystem::path, TextureAsset> mTextureAssets;
		std::unordered_map<std::filesystem::path, MeshAsset> mMeshAssets;
		std::unordered_map<std::filesystem::path, AnimationAsset> mAnimationAssets;
		std::unordered_map<std::filesystem::path, SkeletonAsset> mSkeletonAssets;
		std::unordered_map<std::filesystem::path, PixelShaderAsset> mPixelShaderAssets;
		std::unordered_map<std::filesystem::path, VertexShaderAsset> mVertexShaderAssets;
		std::unordered_map<std::filesystem::path, EntityCompositionAsset> mEntityCompositionAssets;

		std::function<void(AssetManager&)> mDefaultLoader;

		AssetLoader mAssetLoader;
	};
}