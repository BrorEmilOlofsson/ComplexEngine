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
#include "Utility/Shapes/Cylinder.hpp"
#include "Engine/ECS/EntityID.hpp"
#include "Engine/Reflection/DataTypeID.hpp"
#include "Utility/Asset/AssetTypes.hpp"
#include "Graphics/Light/PointLight.hpp"
#include "Graphics/Light/DirectionalLight.hpp"
#include <External/nlohmann/json.hpp>
#include <string>
#include <array>

namespace Simple
{
	void LoadDataPtr(DataTypeID dataTypeID, void* dataPtr, const nlohmann::json& json, const Blackboard& blackboard);
}

void FromJSON(bool& value, const nlohmann::json& json);
void FromJSON(char& value, const nlohmann::json& json);
void FromJSON(int& value, const nlohmann::json& json);
void FromJSON(unsigned int& value, const nlohmann::json& json);
void FromJSON(float& value, const nlohmann::json& json);

namespace std
{
	void FromJSON(std::string& value, const nlohmann::json& json);
}

namespace Simple
{
	template<typename T>
	void FromJSON(Vector2<T>& vector, const nlohmann::json& json)
	{
		vector.x = json.at("x");
		vector.y = json.at("y");
	}

	template<typename T>
	void FromJSON(Vector3<T>& vector, const nlohmann::json& json)
	{
		vector.x = json.at("x");
		vector.y = json.at("y");
		vector.z = json.at("z");
	}

	template<typename T>
	void FromJSON(Vector4<T>& vector, const nlohmann::json& json)
	{
		vector.x = json.at("x");
		vector.y = json.at("y");
		vector.z = json.at("z");
		vector.w = json.at("w");
	}

	template<typename T>
	void FromJSON(Point2<T>& point, const nlohmann::json& json)
	{
		point.x = json.at("x");
		point.y = json.at("y");
	}

	template<typename T>
	void FromJSON(Point3<T>& point, const nlohmann::json& json)
	{
		point.x = json.at("x");
		point.y = json.at("y");
		point.z = json.at("z");
	}

	template<typename T>
	void FromJSON(UnitVector2<T>& vector, const nlohmann::json& json)
	{
		Vector2<T> v;
		FromJSON(v, json);

		vector = UnitVector2<T>(v);
	}

	template<typename T>
	void FromJSON(UnitVector3<T>& vector, const nlohmann::json& json)
	{
		Vector3<T> v;
		FromJSON(v, json);

		vector = UnitVector3<T>(v);
	}

	void FromJSON(Transform& transform, const nlohmann::json& json);
	void FromJSON(Color& color, const nlohmann::json& json);
	void FromJSON(RGBColor& color, const nlohmann::json& json);

	template<typename T>
	void FromJSON(Radians<T>& radians, const nlohmann::json& json)
	{
		::FromJSON(radians.Value(), json);
	}

	template<typename T>
	void FromJSON(Degrees<T>& degrees, const nlohmann::json& json)
	{
		::FromJSON(degrees.Value(), json);
	}

	template<typename T>
	void FromJSON(AABB2<T>& aabb, const nlohmann::json& json)
	{
		Point2<T> min;
		Point2<T> max;
		FromJSON(min, json.at("Min"));
		FromJSON(max, json.at("Max"));
		aabb = AABB2<T>::FromMinAndMax(min, max);
	}

	template<typename T>
	void FromJSON(AABB3<T>& aabb, const nlohmann::json& json)
	{
		Point3<T> min;
		Point3<T> max;
		FromJSON(min, json.at("Min"));
		FromJSON(max, json.at("Max"));
		aabb = AABB3<T>(min, max);
	}

	template<typename T>
    void FromJSON(Cylinder<T>& cylinder, const nlohmann::json& json)
	{
		Point3<T> center;
		UnitVector3<T> axis;
		T radius = T{};
		T height = T{};
		FromJSON(center, json.at("Center"));
		FromJSON(axis, json.at("Axis"));
		::FromJSON(radius, json.at("Radius"));
		::FromJSON(height, json.at("Height"));
		cylinder = Cylinder<T>::FromCenterAndRadiusAndAxisAndHeight(center, Radius<T>(radius), axis, height);
	}

	void FromJSON(EntityID& entityID, const nlohmann::json& json);

	void FromJSON(PointLight& pointLight, const nlohmann::json& json);
	void FromJSON(DirectionalLight& directionalLight, const nlohmann::json& json);
	void FromJSON(Camera& camera, const nlohmann::json& json);

	void FromJSON(MeshAssetHandle& meshAsset, const nlohmann::json& json, const Blackboard& blackboard);
	void FromJSON(ModelAssetHandle& modelAsset, const nlohmann::json& json, const Blackboard& blackboard);
	void FromJSON(AnimatedModelAssetHandle& animatedModelAsset, const nlohmann::json& json, const Blackboard& blackboard);
	void FromJSON(TextureAssetHandle& textureAsset, const nlohmann::json& json, const Blackboard& blackboard);
	void FromJSON(AnimationAssetHandle& animationAsset, const nlohmann::json& json, const Blackboard& blackboard);
	void FromJSON(PixelShaderAssetHandle& shader, const nlohmann::json& json, const Blackboard& blackboard);
	void FromJSON(VertexShaderAssetHandle& shader, const nlohmann::json& json, const Blackboard& blackboard);

	void CustomFromJSON(std::array<TextureAssetHandle, 3>& textureAssets, const nlohmann::json& json, const Blackboard& blackboard);

	template<typename T, size_t N>
	void FromJSON(std::array<T, N>& value, const nlohmann::json& json, const Blackboard& blackboard)
	{
		CustomFromJSON(value, json, blackboard);
	}

	template<typename T> requires std::is_enum_v<T>
	void FromJSON(T& value, const nlohmann::json& json)
	{
		int v = static_cast<int>(value);
		::FromJSON(v, json);
		value = static_cast<T>(v);
	}

	template<typename T>
		requires requires(T& value, const nlohmann::json& json)
	{
		FromJSON(value, json);
	}
	void FromJSON(T& value, const nlohmann::json& json, const Blackboard&)
	{
		return FromJSON(value, json);
	}

}

namespace std
{
	template<typename T>
	void FromJSON(std::vector<T>& vector, const nlohmann::json& json, const Simple::Blackboard& blackboard)
	{
		for (const nlohmann::json& element : json)
		{
			if constexpr (std::same_as<T, bool>)
			{
				vector.emplace_back();
				bool data = vector.emplace_back();
				Simple::LoadDataPtr(Simple::GetDataTypeID<T>(), &data, element, blackboard);
				vector[vector.size() - 1] = data;
			}
			else
			{
				T& data = vector.emplace_back();
				Simple::LoadDataPtr(Simple::GetDataTypeID<T>(), &data, element, blackboard);
			}
		}
	}
}