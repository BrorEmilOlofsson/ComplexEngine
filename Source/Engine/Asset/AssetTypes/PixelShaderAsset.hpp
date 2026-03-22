#pragma once
#include "Engine/Asset/Asset.hpp"
#include "Engine/Asset/AssetHandle.hpp"
#include <filesystem>

#include "Engine/Graphics/Shader/PixelShader.hpp"

namespace CLX
{
	using PixelShaderAsset = Asset<class PixelShader>;
	using PixelShaderAssetHandle = AssetHandle<class PixelShader>;

	enum class ePixelShaderType
	{
		UnlitDefault,
		LitDefault,
		ObjectID,
		FullScreen,
		Line
	};

	constexpr std::string_view EnumToString(ePixelShaderType pixelShaderType)
	{
		switch (pixelShaderType)
		{
		case ePixelShaderType::UnlitDefault:
			return "UnlitDefault";
			break;
		case ePixelShaderType::LitDefault:
			return "LitDefault";
			break;
		case ePixelShaderType::ObjectID:
			return "ObjectID";
			break;
		case ePixelShaderType::FullScreen:
			return "FullScreen";
			break;
		case ePixelShaderType::Line:
			return "Line";
			break;
		default:
			throw std::invalid_argument("Invalid argument pixel shader type");
		}
	}

	inline std::filesystem::path GetPath(const ePixelShaderType type)
	{
		return std::string("Shaders/") + std::string(EnumToString(type)) + std::string("PS.cso");
	}
}