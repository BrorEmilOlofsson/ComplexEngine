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
			if (TextureAsset textureAsset = MapFind(mTextureAssets, std::filesystem::absolute(path)))
			{
				return TextureAssetHandle(textureAsset);
			}
			return TextureAssetHandle();
		}

		[[nodiscard]] MeshAssetHandle GetMesh(const std::filesystem::path& path)
		{
			if (MeshAsset asset = MapFind(mMeshAssets, std::filesystem::absolute(path)))
			{
				return MeshAssetHandle(asset);
			}
			return MeshAssetHandle();
		}

		[[nodiscard]] AnimationAssetHandle GetAnimation(const std::filesystem::path& path)
		{
			if (AnimationAsset asset = MapFind(mAnimationAssets, std::filesystem::absolute(path)))
			{
				return AnimationAssetHandle(asset);
			}
			return AnimationAssetHandle();
		}

		[[nodiscard]] SkeletonAssetHandle GetSkeleton(const std::filesystem::path& path)
		{
			if (SkeletonAsset asset = MapFind(mSkeletonAssets, std::filesystem::absolute(path)))
			{
				return SkeletonAssetHandle(asset);
			}
			return SkeletonAssetHandle();
		}

		[[nodiscard]] PixelShaderAssetHandle GetPixelShader(const std::filesystem::path& path)
		{
			if (PixelShaderAsset asset = MapFind(mPixelShaderAssets, std::filesystem::absolute(path)))
			{
				return PixelShaderAssetHandle(asset);
			}
			return PixelShaderAssetHandle();
		}

		[[nodiscard]] VertexShaderAssetHandle GetVertexShader(const std::filesystem::path& path)
		{
			if (VertexShaderAsset asset = MapFind(mVertexShaderAssets, std::filesystem::absolute(path)))
			{
				return VertexShaderAssetHandle(asset);
			}
			return VertexShaderAssetHandle();
		}


		void AddTexture(const std::filesystem::path& path, TextureAsset textureAsset)
		{
			mTextureAssets[std::filesystem::absolute(path)] = std::move(textureAsset);
		}

		void AddMesh(const std::filesystem::path& path, MeshAsset meshAsset)
		{
			mMeshAssets[std::filesystem::absolute(path)] = std::move(meshAsset);
		}

		void AddPixelShader(const std::filesystem::path& path, PixelShaderAsset asset)
		{
			mPixelShaderAssets[std::filesystem::absolute(path)] = asset;
		}

		void AddVertexShader(const std::filesystem::path& path, VertexShaderAsset asset)
		{
			mVertexShaderAssets[std::filesystem::absolute(path)] = asset;
		}


	private:

		std::unordered_map<std::filesystem::path, TextureAsset> mTextureAssets;
		std::unordered_map<std::filesystem::path, MeshAsset> mMeshAssets;
		std::unordered_map<std::filesystem::path, AnimationAsset> mAnimationAssets;
		std::unordered_map<std::filesystem::path, SkeletonAsset> mSkeletonAssets;
		std::unordered_map<std::filesystem::path, PixelShaderAsset> mPixelShaderAssets;
		std::unordered_map<std::filesystem::path, VertexShaderAsset> mVertexShaderAssets;

		std::function<void(AssetManager&)> mDefaultLoader;

		AssetLoader mAssetLoader;
	};
}