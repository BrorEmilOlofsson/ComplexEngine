#pragma once
#include "Engine/Asset/Asset.hpp"
#include "Engine/Asset/AssetHandle.hpp"
#include <filesystem>

#include "Engine/Graphics/Texture/Texture.hpp"

namespace CLX
{

	using TextureAsset = Asset<class Texture>;
	using TextureAssetHandle = AssetHandle<class Texture>;

	enum class eTextureType
	{
		Default,
		Default_Albedo,
		Default_Normal,
		Default_Material,
		Simple,
		DirectionalLight
	};

	enum class eIconType
	{
		FBX,
		Folder,
		CubeMap,
		PNG,
		JPG,
		OBJ,
		MP3,
		Scene,
		Cursor,
		FlyScript,
		Unknown
	};

	constexpr std::optional<eIconType> ToIconType(const std::string_view str)
	{
		if (str.empty() || str[0] != '.')
		{
			return eIconType::Folder;
		}
		else if (str == ".fbx")
		{
			return eIconType::FBX;
		}
		else if (str == ".scene")
		{
			return eIconType::Scene;
		}
		else if (str == ".png")
		{
			return eIconType::PNG;
		}
		else if (str == ".jpg")
		{
			return eIconType::JPG;
		}
		else if (str == ".obj")
		{
			return eIconType::OBJ;
		}
		else if (str == ".mp3")
		{
			return eIconType::MP3;
		}
		else if (str == ".cur")
		{
			return eIconType::Cursor;
		}
		else if (str == ".fly")
		{
			return eIconType::FlyScript;
		}

		return std::nullopt;
	}

	enum class eSkyBox
	{
		DayCloud,
		NightStar,
		DayGrassland,
		AutumnForest,
		TGA_Skansen
	};

	inline constexpr std::filesystem::path GetPathByTextureType(const eTextureType type)
	{
		auto getPath = [](const std::filesystem::path& relativePath)
			{
				return std::filesystem::path(SIMPLE_DIR_TEXTURES) / relativePath;
			};
		switch (type)
		{
		case eTextureType::Default:
			return getPath("T_SimpleTexture_C.dds");
		case eTextureType::Default_Albedo:
			return getPath("Materials\\T_DefaultMaterial_C.dds");
		case eTextureType::Default_Normal:
			return getPath("Materials\\T_DefaultMaterial_N.dds");
		case eTextureType::Default_Material:
			return getPath("Materials\\T_DefaultMaterial_M.dds");
		case eTextureType::Simple:
			return getPath("T_Hamster_C.dds");
		case eTextureType::DirectionalLight:
			return getPath("T_Sunlight_C.dds");
		}

		return std::filesystem::path();
	}

	inline constexpr std::filesystem::path GetPath(const eSkyBox skyBox)
	{
		auto getPath = [](const std::filesystem::path& relativePath)
		{
			return std::filesystem::path(SIMPLE_DIR_TEXTURES) / relativePath;
		};
		switch (skyBox)
		{
		case eSkyBox::DayCloud:
			return getPath("Cubemaps\\T_DayCloud_E.dds");
		case eSkyBox::NightStar:
			return getPath("Cubemaps\\T_NightStars_E.dds");
		case eSkyBox::DayGrassland:
			return getPath("Cubemaps\\T_CloudAnime_E.dds");
		case eSkyBox::AutumnForest:
			return getPath("Cubemaps\\T_AutumnForest_E.dds");
		case eSkyBox::TGA_Skansen:
			return getPath("Cubemaps\\T_Skansen_E.dds");
		}
		return std::filesystem::path();
	}


	inline constexpr std::filesystem::path GetPath(const eIconType iconType)
	{
		auto getPath = [](const std::filesystem::path& relativePath)
			{
				return std::filesystem::path(SIMPLE_DIR_TEXTURES) / relativePath;
			};
		switch (iconType)
		{
		case eIconType::FBX:
			return getPath("Icons_Editor\\Icon_FBX.dds");
		case eIconType::Folder:
			return getPath("Icons_Editor\\Icon_Folder.dds");
		case eIconType::CubeMap:
			return getPath("Icons_Editor\\Icon_CubeMap.dds");
		case eIconType::PNG:
			return getPath("Icons_Editor\\Icon_PNG.dds");
		case eIconType::JPG:
			return getPath("Icons_Editor\\Icon_JPG.dds");
		case eIconType::OBJ:
			return getPath("Icons_Editor\\Icon_OBJ.dds");
		case eIconType::MP3:
			return getPath("Icons_Editor\\Icon_MP3.dds");
		case eIconType::Scene:
			return getPath("Icons_Editor\\Icon_Scene.dds");
		case eIconType::Cursor:
			return getPath("Icons_Editor\\Icon_Cursor.dds");
		case eIconType::FlyScript:
			return getPath("Icons_Editor\\Icon_FlyScript.dds");
		case eIconType::Unknown:
			return getPath("Icons_Editor\\Icon_Question.dds");
		}
		return std::filesystem::path();
	}
}