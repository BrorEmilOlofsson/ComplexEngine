#pragma once
#include "Utility/Asset/Asset.hpp"
#include "Utility/Asset/AssetHandle.hpp"
#include <filesystem>

namespace Simple
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
		switch (type)
		{
		case eTextureType::Default:
			return std::filesystem::path("Assets\\Textures\\T_SimpleTexture_C.dds");
			break;
		case eTextureType::Default_Albedo:
			return std::filesystem::path("Assets\\Textures\\Materials\\T_DefaultMaterial_C.dds");
			break;
		case eTextureType::Default_Normal:
			return std::filesystem::path("Assets\\Textures\\Materials\\T_DefaultMaterial_N.dds");
			break;
		case eTextureType::Default_Material:
			return std::filesystem::path("Assets\\Textures\\Materials\\T_DefaultMaterial_M.dds");
			break;
		case eTextureType::Simple:
			return std::filesystem::path("Assets\\Textures\\T_Hamster_C.dds");
			break;
		case eTextureType::DirectionalLight:
			return std::filesystem::path("Assets\\Textures\\T_Sunlight_C.dds");
			break;
		}

		return std::filesystem::path();
	}

	inline constexpr std::filesystem::path GetPath(const eSkyBox skyBox)
	{
		switch (skyBox)
		{
		case eSkyBox::DayCloud:
			return std::filesystem::path("Assets\\Textures\\Cubemaps\\T_DayCloud_E.dds");
		case eSkyBox::NightStar:
			return std::filesystem::path("Assets\\Textures\\Cubemaps\\T_NightStars_E.dds");
		case eSkyBox::DayGrassland:
			return std::filesystem::path("Assets\\Textures\\Cubemaps\\T_CloudAnime_E.dds");
		case eSkyBox::AutumnForest:
			return std::filesystem::path("Assets\\Textures\\Cubemaps\\T_AutumnForest_E.dds");
		case eSkyBox::TGA_Skansen:
			return std::filesystem::path("Assets\\Textures\\Cubemaps\\T_Skansen_E.dds");
		}
		return std::filesystem::path();
	}


	inline constexpr std::filesystem::path GetPath(const eIconType iconType)
	{
		switch (iconType)
		{
		case eIconType::FBX:
			return std::filesystem::path("Assets\\Textures\\Icons_Editor\\Icon_FBX.dds");
		case eIconType::Folder:
			return std::filesystem::path("Assets\\Textures\\Icons_Editor\\Icon_Folder.dds");
		case eIconType::CubeMap:
			return std::filesystem::path("Assets\\Textures\\Icons_Editor\\Icon_CubeMap.dds");
		case eIconType::PNG:
			return std::filesystem::path("Assets\\Textures\\Icons_Editor\\Icon_PNG.dds");
		case eIconType::JPG:
			return std::filesystem::path("Assets\\Textures\\Icons_Editor\\Icon_JPG.dds");
		case eIconType::OBJ:
			return std::filesystem::path("Assets\\Textures\\Icons_Editor\\Icon_OBJ.dds");
		case eIconType::MP3:
			return std::filesystem::path("Assets\\Textures\\Icons_Editor\\Icon_MP3.dds");
		case eIconType::Scene:
			return std::filesystem::path("Assets\\Textures\\Icons_Editor\\Icon_Scene.dds");
		case eIconType::Cursor:
			return std::filesystem::path("Assets\\Textures\\Icons_Editor\\Icon_Cursor.dds");
		case eIconType::FlyScript:
			return std::filesystem::path("Assets\\Textures\\Icons_Editor\\Icon_FlyScript.dds");
		case eIconType::Unknown:
			return std::filesystem::path("Assets\\Textures\\Icons_Editor\\Icon_Question.dds");
		}
		return std::filesystem::path();
	}
}