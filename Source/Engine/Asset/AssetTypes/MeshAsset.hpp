#pragma once
#include "Engine/Asset/Asset.hpp"
#include "Engine/Asset/AssetHandle.hpp"
#include <filesystem>

#include "Engine/Graphics/Mesh/Mesh.hpp"

namespace CLX
{
	using MeshAsset = Asset<class Mesh>;
	using MeshAssetHandle = AssetHandle<class Mesh>;

	enum class ePrimitiveShape
	{
		Cube,
		Pyramid,
		Plane,
		Sphere,
		SkyBox
	};

	constexpr std::string_view ToString(const ePrimitiveShape type)
	{
		switch (type)
		{
		case ePrimitiveShape::Cube:
			return "Primitive Cube";
		case ePrimitiveShape::Pyramid:
			return "Primitive Pyramid";
		case ePrimitiveShape::Plane:
			return "Primitive Plane";
		case ePrimitiveShape::Sphere:
			return "Primitive Sphere";
		case ePrimitiveShape::SkyBox:
			return "Primitive SkyBox";
		default:
			throw std::invalid_argument("Invalid primitive shape type");
			break;
		}
	}

	inline std::filesystem::path GetPathByPrimitiveMeshType(const ePrimitiveShape type)
	{
		switch (type)
		{
		case ePrimitiveShape::Cube:
			return "Primitive Cube";
		case ePrimitiveShape::Pyramid:
			return "Primitive Pyramid";
		case ePrimitiveShape::Plane:
			return "Primitive Plane";
		case ePrimitiveShape::Sphere:
			return "Primitive Sphere";
		case ePrimitiveShape::SkyBox:
			return "Primitive SkyBox";
		default:
			throw std::invalid_argument("Invalid primitive shape type");
			break;
		}
	}
}