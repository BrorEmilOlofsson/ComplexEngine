#pragma once
#include <tuple>
#include <string_view>
#include "Engine/Utility/EnumUtility.hpp"

namespace Simple
{
	//using MeshTextureKey = std::tuple<const DX11Mesh*, const class Texture*, const class Texture*, const class Texture*>;

	constexpr std::size_t GLOBAL_INSTANCER_MAX_INSTANCE = 1024;

	enum class eRasterizerState
	{
		BackfaceCulling,
		NoFaceCulling,
		Wireframe,
		WireframeNoCulling,
		FrontFaceCulling,
		Count
	};

	template<>
	struct EnumCount<eRasterizerState> : std::integral_constant<std::size_t, 5> {};

	constexpr std::string_view EnumToString(eRasterizerState rasterizerState)
	{
		switch (rasterizerState)
		{
		case eRasterizerState::BackfaceCulling:
			return "BackfaceCulling";
			break;
		case eRasterizerState::NoFaceCulling:
			return "NoFaceCulling";
			break;
		case eRasterizerState::Wireframe:
			return "Wireframe";
			break;
		case eRasterizerState::WireframeNoCulling:
			return "WireframeNoCulling";
			break;
		case eRasterizerState::FrontFaceCulling:
			return "FrontFaceCulling";
			break;
		default:
			return "None";
			break;
		}
	}

	enum class eBlendState
	{
		Disabled,
		AlphaBlend,
		AdditiveBlend,
		Count
	};

	enum class eSamplerState
	{
		Bilinear_Warp,
		Trilinear_Clamp,
		Count
	};

	enum class eDepthStencilState
	{
		Less_Equal,
		Greater,
		Count
	};

	enum class eRenderTargetType
	{
		Backbuffer,
		GBuffer,
		Deferred,
		PostProcessing,
		BloomDownAndUpScale,
		Bloom,
		Count
	};

	/*struct MeshTextureKeyHash
	{
		std::size_t operator()(const MeshTextureKey& key) const
		{
			return
				std::hash<const DX11Mesh*>()(std::get<0>(key)) ^
				std::hash<const class Texture*>()(std::get<1>(key)) ^
				std::hash<const class Texture*>()(std::get<2>(key)) ^
				std::hash<const class Texture*>()(std::get<3>(key));
		}
	};*/
}