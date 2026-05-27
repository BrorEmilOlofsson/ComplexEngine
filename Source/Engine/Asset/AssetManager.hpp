#pragma once
#include <filesystem>
#include <functional>
#include <fstream>
#include <unordered_map>
#include "Engine/Asset/AssetTypes/AssetTypes.hpp"
#include "Engine/Asset/AssetLoader.hpp"
#include "Engine/Utility/File/FileUtility.hpp"
#include "Engine/Asset/AssetExtensions.hpp"

namespace CLX
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
        template<typename HandleType, bool IsAssetPath>
        [[nodiscard]] static constexpr decltype(auto) ValidatedGet(auto& path, auto& map)
        {
            const std::filesystem::path absolutePath = IsAssetPath ? std::filesystem::absolute(SIMPLE_DIR_ASSETS / path) : std::filesystem::absolute(path);
            if (auto asset = MapFind(map, absolutePath))
            {
                return HandleType(asset);
            }
            return HandleType();
        }
    public:

        AssetManager() = default;

        void SetDefaultLoader(std::function<void(AssetManager&)> defaultLoader)
        {
            mDefaultLoader = std::move(defaultLoader);
        }

        [[nodiscard]] AssetLoader& GetAssetLoader()
        {
            return mAssetLoader;
        }

        [[nodiscard]] const AssetLoader& GetAssetLoader() const
        {
            return mAssetLoader;
        }

        void LoadAssets();

        [[nodiscard]] TextureAssetHandle GetTexture(const std::filesystem::path& path)
        {
            return ValidatedGet<TextureAssetHandle, true>(path, mTextureAssets);
        }

        [[nodiscard]] MeshAssetHandle GetMesh(const std::filesystem::path& path)
        {
            return ValidatedGet<MeshAssetHandle, true>(path, mMeshAssets);
        }

        [[nodiscard]] ModelAssetHandle GetModel(const std::filesystem::path& path)
        {
            return ValidatedGet<ModelAssetHandle, true>(path, mModelAssets);
        }

        [[nodiscard]] AnimatedModelAssetHandle GetAnimatedModel(const std::filesystem::path& path)
        {
            return ValidatedGet<AnimatedModelAssetHandle, true>(path, mAnimatedModelAssets);
        }

        [[nodiscard]] AnimationAssetHandle GetAnimation(const std::filesystem::path& path)
        {
            return ValidatedGet<AnimationAssetHandle, true>(path, mAnimationAssets);
        }

        [[nodiscard]] PixelShaderAssetHandle GetPixelShader(const std::filesystem::path& path)
        {
            return ValidatedGet<PixelShaderAssetHandle, false>(path, mPixelShaderAssets);
        }

        [[nodiscard]] VertexShaderAssetHandle GetVertexShader(const std::filesystem::path& path)
        {
            return ValidatedGet<VertexShaderAssetHandle, false>(path, mVertexShaderAssets);
        }

        [[nodiscard]] SceneAssetHandle GetScene(const std::filesystem::path& path)
        {
            return ValidatedGet<SceneAssetHandle, true>(path, mSceneAssets);
        }

        [[nodiscard]] AudioAssetHandle GetAudio(const std::filesystem::path& path)
        {
            return ValidatedGet<AudioAssetHandle, true>(path, mAudioAssets);
        }

        template<typename T>
        [[nodiscard]] auto GetAsset(const std::filesystem::path& path)
        {
            if constexpr (std::same_as<T, Texture>)
            {
                return GetTexture(path);
            }
            else if constexpr (std::same_as<T, Mesh>)
            {
                return GetMesh(path);
            }
            else if constexpr (std::same_as<T, Model>)
            {
                return GetModel(path);
            }
            else if constexpr (std::same_as<T, AnimatedModel>)
            {
                return GetAnimatedModel(path);
            }
            else if constexpr (std::same_as<T, Animation>)
            {
                return GetAnimation(path);
            }
            else if constexpr (std::same_as<T, PixelShader>)
            {
                return GetPixelShader(path);
            }
            else if constexpr (std::same_as<T, VertexShader>)
            {
                return GetVertexShader(path);
            }
            else if constexpr (std::same_as<T, Scene>)
            {
                return GetScene(path);
            }
            else if constexpr (std::same_as<T, EntityComposition>)
            {
                return GetEntityComposition(path);
            }
            else if constexpr (std::same_as<T, Audio>)
            {
                return GetAudio(path);
            }
            else
            {
                static_assert(false, "Unsupported asset type");
            }
        }

        template<typename T>
        [[nodiscard]] decltype(auto) GetAssets()
        {
            if constexpr (std::same_as<T, Texture>)
            {
                return mTextureAssets;
            }
            else if constexpr (std::same_as<T, Mesh>)
            {
                return mMeshAssets;
            }
            else if constexpr (std::same_as<T, Model>)
            {
                return mModelAssets;
            }
            else if constexpr (std::same_as<T, AnimatedModel>)
            {
                return mAnimatedModelAssets;
            }
            else if constexpr (std::same_as<T, Animation>)
            {
                return mAnimationAssets;
            }
            else if constexpr (std::same_as<T, PixelShader>)
            {
                return mPixelShaderAssets;
            }
            else if constexpr (std::same_as<T, VertexShader>)
            {
                return mVertexShaderAssets;
            }
            else if constexpr (std::same_as<T, Scene>)
            {
                return mSceneAssets;
            }
            else if constexpr (std::same_as<T, EntityComposition>)
            {
                return mEntityCompositionAssets;
            }
            else if constexpr (std::same_as<T, Audio>)
            {
                return mAudioAssets;
            }
            else
            {
                static_assert(false, "Unsupported asset type");
            }
        }

        [[nodiscard]] EntityCompositionAssetHandle GetEntityComposition(const std::filesystem::path& path)
        {
            return ValidatedGet<EntityCompositionAssetHandle, true>(path, mEntityCompositionAssets);
        }

        void AddTexture(const std::filesystem::path& path, TextureAsset asset)
        {
            if (!asset)
            {
                throw std::invalid_argument("Asset is invalid");
            }
            mTextureAssets[std::filesystem::absolute(path)] = std::move(asset);
        }

        void AddModel(const std::filesystem::path& path, ModelAsset asset)
        {
            if (!asset)
            {
                throw std::invalid_argument("Asset is invalid");
            }
            mModelAssets[std::filesystem::absolute(path)] = std::move(asset);
        }

        void AddAnimatedModel(const std::filesystem::path& path, AnimatedModelAsset asset)
        {
            if (!asset)
            {
                throw std::invalid_argument("Asset is invalid");
            }
            mAnimatedModelAssets[std::filesystem::absolute(path)] = std::move(asset);
        }

        void AddAnimation(const std::filesystem::path& path, AnimationAsset asset)
        {
            if (!asset)
            {
                throw std::invalid_argument("Asset is invalid");
            }
            mAnimationAssets[std::filesystem::absolute(path)] = std::move(asset);
        }

        void AddMesh(const std::filesystem::path& path, MeshAsset asset)
        {
            if (!asset)
            {
                throw std::invalid_argument("Asset is invalid");
            }
            mMeshAssets[GetAbsoluteAssetPath(path)] = std::move(asset);
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

        void AddScene(const std::filesystem::path& path, SceneAsset asset)
        {
            if (!asset)
            {
                throw std::invalid_argument("Asset is invalid");
            }
            mSceneAssets[std::filesystem::absolute(path)] = std::move(asset);
        }

        EntityCompositionAssetHandle AddEntityComposition(EntityCompositionAsset asset)
        {
            if (!asset)
            {
                throw std::invalid_argument("Asset is invalid");
            }
            const std::filesystem::path path = asset.GetRelativePath();
            mEntityCompositionAssets[std::filesystem::absolute(path)] = std::move(asset);
            return GetEntityComposition(path);
        }

        void AddAudio(const std::filesystem::path& path, AudioAsset asset)
        {
            if (!asset)
            {
                throw std::invalid_argument("Asset is invalid");
            }
            mAudioAssets[std::filesystem::absolute(path)] = std::move(asset);
        }

        SceneAssetHandle CreateScene(const std::filesystem::path& path)
        {
            if (path.extension() != AssetExtensions::Scene)
            {
                throw std::invalid_argument("Invalid scene file extension: " + path.extension().string());
            }


            if (std::filesystem::exists(path))
            {
                //
            }

            std::ofstream file(path, std::ios::out);

            if (!file.is_open())
            {
                throw std::runtime_error("Failed to create scene file at path: " + path.string());
            }
            
            file << "{}";
            file.close();

            LoadAsset(path);
            return GetScene(path);
        }

        void LoadAsset(const std::filesystem::path& path)
        {
            const auto extension = path.extension();
            if (extension == AssetExtensions::EntityComposition)
            {
                EntityCompositionAsset asset = mAssetLoader.LoadEntityComposition(path);
                AddEntityComposition(std::move(asset));
            }
            else if (extension == AssetExtensions::Scene)
            {
                SceneAsset asset = mAssetLoader.LoadScene(path);
                AddScene(path, std::move(asset));
            }
            else
            {
                throw std::invalid_argument("Unsupported asset type: " + extension.string());
            }
        }

        void Remove(const std::filesystem::path& path)
        {
            const auto extension = path.extension();

            auto remove = [](auto& map, const std::filesystem::path& path)
                {
                    map.erase(path);
                };

            if (extension == ".dds")
            {
                remove(mTextureAssets, path);
            }
            else if (extension == ".fbx")
            {
                remove(mMeshAssets, path);
                remove(mModelAssets, path);
            }
            else if (extension == ".cso")
            {
                remove(mPixelShaderAssets, path);
                remove(mVertexShaderAssets, path);
            }
            else if (extension == AssetExtensions::EntityComposition)
            {
                remove(mEntityCompositionAssets, path);
            }
            else
            {
                ASSERT(false);
            }
        }

        void Rename(const std::filesystem::path& path, const std::string& newName)
        {
            const std::filesystem::path extension = path.extension();
            const std::filesystem::path newPath = path.parent_path() / (newName + extension.string());

            if (path == newPath)
            {
                return;
            }

            auto rename = [](auto&& map, const std::filesystem::path & oldPath, const std::filesystem::path & newPath)
            {
                auto entityCompositionAsset = MapFind(map, oldPath);

                if (entityCompositionAsset)
                {
                    entityCompositionAsset.SetRelativePath(newPath);
                    map.erase(oldPath);
                    map[newPath] = std::move(entityCompositionAsset);
                }

                std::filesystem::rename(oldPath, newPath);
            };

            if (extension == AssetExtensions::EntityComposition)
            {
                rename(mEntityCompositionAssets, path, newPath);
            }
            else
            {
                ASSERT(false);
            }
        }

    private:

        std::unordered_map<std::filesystem::path, TextureAsset> mTextureAssets;
        std::unordered_map<std::filesystem::path, MeshAsset> mMeshAssets;
        std::unordered_map<std::filesystem::path, ModelAsset> mModelAssets;
        std::unordered_map<std::filesystem::path, AnimatedModelAsset> mAnimatedModelAssets;
        std::unordered_map<std::filesystem::path, AnimationAsset> mAnimationAssets;
        std::unordered_map<std::filesystem::path, PixelShaderAsset> mPixelShaderAssets;
        std::unordered_map<std::filesystem::path, VertexShaderAsset> mVertexShaderAssets;
        std::unordered_map<std::filesystem::path, SceneAsset> mSceneAssets;
        std::unordered_map<std::filesystem::path, EntityCompositionAsset> mEntityCompositionAssets;
        std::unordered_map<std::filesystem::path, AudioAsset> mAudioAssets;

        std::function<void(AssetManager&)> mDefaultLoader;

        AssetLoader mAssetLoader;
    };
}