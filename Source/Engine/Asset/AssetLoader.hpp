#pragma once
#include <functional>
#include <filesystem>
#include <variant>
#include <vector>
#include "Engine/Asset/AssetTypes/AssetTypes.hpp"

namespace CLX
{

	struct FBXParseResult
	{
		std::variant<std::monostate, ModelAsset, AnimatedModelAsset> model;
		std::vector<AnimationAsset> animations;
	};

	class AssetLoader final
	{
	public:

		AssetLoader() = default;

		[[nodiscard]] TextureAsset LoadTexture(const std::filesystem::path& path) const
		{
			return mTextureLoader(path);
		}

		[[nodiscard]] MeshAsset LoadMesh(const std::filesystem::path& path) const
		{
			return mMeshLoader(path);
		}

		[[nodiscard]] FBXParseResult LoadFBX(const std::filesystem::path& path) const
		{
			return mFBXLoader(path);
		}

		[[nodiscard]] PixelShaderAsset LoadPixelShader(const std::filesystem::path& path) const
		{
			return mPixelShaderLoader(path);
		}

		[[nodiscard]] VertexShaderAsset LoadVertexShader(const std::filesystem::path& path) const
		{
			return mVertexShaderLoader(path);
		}

		[[nodiscard]] EntityCompositionAsset LoadEntityComposition(const std::filesystem::path& path) const
		{
			return mEntityCompositionLoader(path);
		}

		[[nodiscard]] SceneAsset LoadScene(const std::filesystem::path& path) const
		{
			return mSceneLoader(path);
		}

        [[nodiscard]] AudioAsset LoadAudio(const std::filesystem::path& path) const
        {
            return mAudioLoader(path);
        }

		void SetTextureLoader(std::function<TextureAsset(const std::filesystem::path&)> loader)
		{
			mTextureLoader = std::move(loader);
		}

		void SetMeshLoader(std::function<MeshAsset(const std::filesystem::path&)> loader)
		{
			mMeshLoader = std::move(loader);
		}

		void SetFBXLoader(std::function<FBXParseResult(const std::filesystem::path&)> loader)
		{
			mFBXLoader = std::move(loader);
		}

		void SetPixelShaderLoader(std::function<PixelShaderAsset(const std::filesystem::path&)> loader)
		{
			mPixelShaderLoader = std::move(loader);
		}

		void SetVertexShaderLoader(std::function<VertexShaderAsset(const std::filesystem::path&)> loader)
		{
			mVertexShaderLoader = std::move(loader);
		}

		void SetEntityCompositionLoader(std::function<EntityCompositionAsset(const std::filesystem::path&)> loader)
		{
			mEntityCompositionLoader = std::move(loader);
		}

		void SetSceneLoader(std::function<SceneAsset(const std::filesystem::path&)> loader)
		{
			mSceneLoader = std::move(loader);
        }

		void SetAudioLoader(std::function<AudioAsset(const std::filesystem::path&)> loader)
		{
			mAudioLoader = std::move(loader);
		}

	private:

		std::function<TextureAsset(const std::filesystem::path&)> mTextureLoader;
		std::function<MeshAsset(const std::filesystem::path&)> mMeshLoader;
		std::function<FBXParseResult(const std::filesystem::path&)> mFBXLoader;
		std::function<PixelShaderAsset(const std::filesystem::path&)> mPixelShaderLoader;
		std::function<VertexShaderAsset(const std::filesystem::path&)> mVertexShaderLoader;
		std::function<EntityCompositionAsset(const std::filesystem::path&)> mEntityCompositionLoader;
        std::function<SceneAsset(const std::filesystem::path&)> mSceneLoader;
        std::function<AudioAsset(const std::filesystem::path&)> mAudioLoader;
	};
}