#pragma once
#include "Utility/Math/Vector2.hpp"
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/Vector4.hpp"
#include "Utility/Math/UnitVector2.hpp"
#include "Utility/Math/UnitVector3.hpp"
#include "Utility/Color.hpp"
#include "Utility/RGBColor.hpp"
#include "Utility/Shapes/AABB2.hpp"
#include "Utility/Shapes/AABB3.hpp"
#include "Engine/ECS/EntityID.hpp"
#include "Engine/Reflection/DataTypeID.hpp"
#include "Utility/Asset/AssetTypes.hpp"
#include "Graphics/Light/PointLight.hpp"
#include "Graphics/Light/DirectionalLight.hpp"
#include <External/nlohmann/json.hpp>
#include <string>
#include <array>

[[nodiscard]] nlohmann::json ToJSON(const bool& value);
[[nodiscard]] nlohmann::json ToJSON(const char& value);
[[nodiscard]] nlohmann::json ToJSON(const int& value);
[[nodiscard]] nlohmann::json ToJSON(const unsigned int& value);
[[nodiscard]] nlohmann::json ToJSON(const float& value);

namespace std
{
	[[nodiscard]] nlohmann::json ToJSON(const std::string& value);
}

namespace Simple
{
	[[nodiscard]] nlohmann::json SaveDataPtr(const DataTypeID dataTypeID, const void* dataPtr);

	[[nodiscard]] nlohmann::json ToJSON(const Transform& transform);

	template<typename T>
	[[nodiscard]] nlohmann::json ToJSON(const Vector2<T>& vector)
	{
		nlohmann::json json;

		json["x"] = vector.x;
		json["y"] = vector.y;

		return json;
	}

	template<typename T>
	[[nodiscard]] nlohmann::json ToJSON(const Vector3<T>& vector)
	{
		nlohmann::json json;


		json["x"] = vector.x;
		json["y"] = vector.y;
		json["z"] = vector.z;

		return json;
	}

	template<typename T>
	[[nodiscard]] nlohmann::json ToJSON(const Vector4<T>& vector)
	{
		nlohmann::json json;

		json["x"] = vector.x;
		json["y"] = vector.y;
		json["z"] = vector.z;
		json["w"] = vector.w;

		return json;
	}

	template<typename T>
	[[nodiscard]] nlohmann::json ToJSON(const Point2<T>& point)
	{
		nlohmann::json json;

		json["x"] = point.x;
		json["y"] = point.y;

		return json;
	}

	template<typename T>
	[[nodiscard]] nlohmann::json ToJSON(const Point3<T>& point)
	{
		nlohmann::json json;

		json["x"] = point.x;
		json["y"] = point.y;
		json["z"] = point.z;

		return json;
	}

	template<typename T>
	[[nodiscard]] nlohmann::json ToJSON(const UnitVector2<T>& vector)
	{
		nlohmann::json json;

		json["x"] = vector.X();
		json["y"] = vector.Y();

		return json;
	}

	template<typename T>
	[[nodiscard]] nlohmann::json ToJSON(const UnitVector3<T>& vector)
	{
		nlohmann::json json;

		json["x"] = vector.X();
		json["y"] = vector.Y();
		json["z"] = vector.Z();

		return json;
	}

	template<typename T>
	[[nodiscard]] nlohmann::json ToJSON(const AABB2<T>& aabb)
	{
		nlohmann::json json;

		json["Min"] = ToJSON(aabb.GetMin());
		json["Max"] = ToJSON(aabb.GetMax());

		return json;
	}

	template<typename T>
	[[nodiscard]] nlohmann::json ToJSON(const AABB3<T>& aabb)
	{
		nlohmann::json json;

		json["Min"] = ToJSON(aabb.GetMin());
		json["Max"] = ToJSON(aabb.GetMax());

		return json;
	}

	[[nodiscard]] nlohmann::json ToJSON(const Color& color);
	[[nodiscard]] nlohmann::json ToJSON(const RGBColor& color);

	template<typename T>
	[[nodiscard]] nlohmann::json ToJSON(const Radians<T>& radians)
	{
		return ::ToJSON(radians.Value());
	}
	template<typename T>
	[[nodiscard]] nlohmann::json ToJSON(const Degrees<T>& degrees)
	{
		return ::ToJSON(degrees.Value());
	}

	[[nodiscard]] nlohmann::json ToJSON(const EntityID& entityID);

	[[nodiscard]] nlohmann::json ToJSON(const PointLight& pointlight);
	[[nodiscard]] nlohmann::json ToJSON(const DirectionalLight& directionalLight);
	[[nodiscard]] nlohmann::json ToJSON(const Camera& camera);

	[[nodiscard]] nlohmann::json ToJSON(const MeshAssetHandle& meshAsset);
	[[nodiscard]] nlohmann::json ToJSON(const ModelAssetHandle& modelAsset);
	[[nodiscard]] nlohmann::json ToJSON(const PixelShaderAssetHandle& shaderAsset);
	[[nodiscard]] nlohmann::json ToJSON(const VertexShaderAssetHandle& shaderAsset);
	[[nodiscard]] nlohmann::json ToJSON(const TextureAssetHandle& textureAsset);
	[[nodiscard]] nlohmann::json ToJSON(const SkeletonAssetHandle& skeletonAsset);
	[[nodiscard]] nlohmann::json ToJSON(const AnimationAssetHandle& animationAsset);

	[[nodiscard]] nlohmann::json CustomToJSON(const std::array<TextureAssetHandle, 3>& textureAssets);

	template<typename T, size_t N>
	[[nodiscard]] nlohmann::json ToJSON(const std::array<T, N>& value)
	{
		return CustomToJSON(value);
	}

	template<typename T> requires std::is_enum_v<T>
	[[nodiscard]] nlohmann::json ToJSON(const T& value)
	{
		return ::ToJSON(static_cast<int>(value));
	}


}

namespace std
{
	template<typename T>
	[[nodiscard]] nlohmann::json ToJSON(const std::vector<T>& vector)
	{
		nlohmann::json arrayJson = nlohmann::json::array();
		for (const T& data : vector)
		{
			nlohmann::json element = Simple::SaveDataPtr(Simple::GetDataTypeID<T>(), &data);

			arrayJson.push_back(element);
		}
		return arrayJson;
	}
}