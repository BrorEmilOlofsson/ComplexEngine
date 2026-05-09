#pragma once
#include <filesystem>
#include "Engine/Asset/Asset.hpp"
#include "Engine/Asset/AssetHandle.hpp"

#include "Engine/Graphics/Shader/VertexShader.hpp"

namespace CLX
{
	using VertexShaderAsset = Asset<class VertexShader>;
	using VertexShaderAssetHandle = AssetHandle<class VertexShader>;

	enum class eVertexShaderType
	{
		Default,
		InstancedDefault,
		AnimatedModel,
		FullScreen
	};

	constexpr std::string_view ToString(eVertexShaderType type)
	{
		switch (type)
		{
		case eVertexShaderType::Default:
			return "Default";
			break;
		case eVertexShaderType::FullScreen:
			return "FullScreen";
			break;
		default:
			throw std::invalid_argument("Invalid argument vertex shader type");
		}
	}

	inline std::filesystem::path GetPath(const eVertexShaderType type)
	{
		return std::string("Shaders/") + std::string(ToString(type)) + std::string("VS.cso");
	}
}