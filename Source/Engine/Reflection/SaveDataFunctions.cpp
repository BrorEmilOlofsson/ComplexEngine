#include "Engine/Precompiled/EnginePch.hpp"
#include "SaveDataFunctions.hpp"
#include "Engine/Math/Transform3.hpp"
#include "Engine/Graphics/Animation/Animation.hpp"
#include "Engine/Utility/Camera.hpp"
#include "Engine/Graphics/Mesh/Mesh.hpp"
#include "Engine/Graphics/Model/Model.hpp"
#include "Engine/Graphics/Model/AnimatedModel.hpp"
#include "Engine/Graphics/GraphicsConstants.hpp"
#include "Engine/Reflection/DataTypeRegistry.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Reflection/VariantReflection.hpp"

namespace CLX
{
	nlohmann::json SaveDataPtr(const DataTypeID dataTypeID, const void* dataPtr, const DataTypeRegistry& dataTypeRegistry)
	{
		return dataTypeRegistry.SaveDataJSON(dataTypeID, dataPtr);
	}
}

nlohmann::json ToJSON(const bool& value)
{
	return nlohmann::json(value);
}

nlohmann::json ToJSON(const char& value)
{
	return ToJSON(static_cast<int>(value));
}

nlohmann::json ToJSON(const int& value)
{
	return nlohmann::json(value);
}

nlohmann::json ToJSON(const unsigned int& value)
{
	return nlohmann::json(value);
}

nlohmann::json ToJSON(const float& value)
{
	return nlohmann::json(value);
}

namespace std
{
	nlohmann::json ToJSON(const std::string& value)
	{
		return nlohmann::json(value);
	}
}

namespace CLX
{
	nlohmann::json ToJSON(const Transform& transform)
	{
		nlohmann::json json;

		const Point3f position = transform.GetPosition();
		const Rotatorf rotation = ToRotator(transform.GetRotation());
		const Vector3f scale = transform.GetScale();

		json["Position"]["x"] = position.x;
		json["Position"]["y"] = position.y;
		json["Position"]["z"] = position.z;

		json["Rotation"]["x"] = rotation.Pitch().Value();
		json["Rotation"]["y"] = rotation.Yaw().Value();
		json["Rotation"]["z"] = rotation.Roll().Value();

		json["Scale"]["x"] = scale.x;
		json["Scale"]["y"] = scale.y;
		json["Scale"]["z"] = scale.z;

		return json;
	}

	nlohmann::json ToJSON(const Vector2f& vector)
	{
		nlohmann::json json;

		json["x"] = vector.x;
		json["y"] = vector.y;

		return json;
	}

	nlohmann::json ToJSON(const Vector3f& vector)
	{
		nlohmann::json json;

		json["x"] = vector.x;
		json["y"] = vector.y;
		json["z"] = vector.z;

		return json;
	}

	nlohmann::json ToJSON(const Vector4f& vector)
	{
		nlohmann::json json;

		json["x"] = vector.x;
		json["y"] = vector.y;
		json["z"] = vector.z;
		json["w"] = vector.w;

		return json;
	}

	nlohmann::json ToJSON(const Point2f& point)
	{
		nlohmann::json json;

		json["x"] = point.x;
		json["y"] = point.y;

		return json;
	}

	nlohmann::json ToJSON(const Point3f& point)
	{
		nlohmann::json json;

		json["x"] = point.x;
		json["y"] = point.y;
		json["z"] = point.z;

		return json;
	}

	nlohmann::json ToJSON(const UnitVector2f& vector)
	{
		nlohmann::json json;

		json["x"] = vector.X();
		json["y"] = vector.Y();

		return json;
	}

	nlohmann::json ToJSON(const UnitVector3f& vector)
	{
		nlohmann::json json;

		json["x"] = vector.X();
		json["y"] = vector.Y();
		json["z"] = vector.Z();

		return json;
	}

	nlohmann::json ToJSON(const Color& color)
	{
		nlohmann::json json;

		json["r"] = color.r;
		json["g"] = color.g;
		json["b"] = color.b;
		json["a"] = color.a;

		return json;
	}

	nlohmann::json ToJSON(const RGBColor& color)
	{
		nlohmann::json json;

		json["r"] = color.r;
		json["g"] = color.g;
		json["b"] = color.b;

		return json;
	}

	nlohmann::json ToJSON(const EntityID& entityID)
	{
		return ::ToJSON(entityID.id);
	}

	nlohmann::json ToJSON(const PointLight& pointLight)
	{
		nlohmann::json json;

		json["Color"] = ToJSON(pointLight.color);
		json["Intensity"] = pointLight.intensity;
		json["Range"] = pointLight.range;

		return json;
	}

	nlohmann::json ToJSON(const DirectionalLight& directionalLight)
	{
		nlohmann::json json;
		json["Direction"] = ToJSON(directionalLight.direction);
		json["Color"] = ToJSON(directionalLight.color);
		json["Intensity"] = directionalLight.intensity;
		return json;
	}

	nlohmann::json ToJSON(const Camera& camera)
	{
		nlohmann::json json;

		json["Position"] = ToJSON(camera.GetPosition());

		json["HorizontalFoV"] = camera.GetHorizontalFOV().Value();
		json["NearPlane"] = camera.GetNearPlane();
		json["FarPlane"] = camera.GetFarPlane();

		return json;
	}

	nlohmann::json ToJSON(const Shape& shape, const DataTypeRegistry& dataTypeRegistry)
	{
		auto variantInfo = GetVariantInfo(shape);
		nlohmann::json json;
        json["ShapeType"] = dataTypeRegistry.Find(variantInfo.first)->name;
        json["ShapeData"] = dataTypeRegistry.SaveDataJSON(variantInfo.first, variantInfo.second);
		return json;
	}

	nlohmann::json ToJSON(const MeshAssetHandle& meshAsset)
	{
		nlohmann::json json;
		std::filesystem::path meshPath;

		if (meshAsset)
		{
			meshPath = meshAsset->GetPath();
		}

		json = meshPath;
		return json;
	}

	nlohmann::json ToJSON(const ModelAssetHandle& modelAsset)
	{
		nlohmann::json json;
		std::filesystem::path meshPath;

		if (modelAsset)
		{
			meshPath = modelAsset->GetPath();
		}

		json = meshPath;
		return json;
	}

	nlohmann::json ToJSON(const AnimatedModelAssetHandle& asset)
	{
		nlohmann::json json;
		std::filesystem::path meshPath;

		if (asset)
		{
			meshPath = asset->GetPath();
		}

		json = meshPath;
		return json;
	}

	nlohmann::json ToJSON(const TextureAssetHandle& textureAsset)
	{
		nlohmann::json json;

		std::filesystem::path textureName;

		if (textureAsset)
		{
			textureName = textureAsset.GetRelativePath();
		}

		json = textureName;

		return json;
	}

	nlohmann::json ToJSON(const PixelShaderAssetHandle& pixelShader)
	{
		nlohmann::json json;

		std::filesystem::path pixelShaderPath;

		if (pixelShader)
		{
			pixelShaderPath = pixelShader.GetRelativePath();
		}

		json["PixelShader"] = pixelShaderPath;

		return json;
	}

	nlohmann::json ToJSON(const VertexShaderAssetHandle& vertexShader)
	{
		nlohmann::json json;

		std::filesystem::path vertexShaderPath;

		if (vertexShader)
		{
			vertexShaderPath = vertexShader.GetRelativePath();
		}

		json["VertexShader"] = vertexShaderPath;

		return json;
	}

	nlohmann::json ToJSON(const AnimationAssetHandle& animationAsset)
	{
		nlohmann::json json;

		std::filesystem::path animationRelativePath;

		if (animationAsset)
		{
			animationRelativePath = animationAsset->path;
		}

		json = animationRelativePath;

		return json;
	}

	nlohmann::json ToJSON(const SceneAssetHandle& sceneAsset)
	{
		nlohmann::json json;
		std::filesystem::path sceneRelativePath;
		if (sceneAsset)
		{
			sceneRelativePath = sceneAsset.GetRelativePath();
		}
		json["ScenePath"] = sceneRelativePath;
		return json;
    }

	nlohmann::json CustomToJSON(const std::array<TextureAssetHandle, 3>& textureAssets)
	{
		nlohmann::json json;

		TextureAssetHandle albedoTextureHandle = textureAssets[TextureSlots::Albedo];
		TextureAssetHandle normalTextureHandle = textureAssets[TextureSlots::Normal];
		TextureAssetHandle materialTextureHandle = textureAssets[TextureSlots::Material];
		const std::filesystem::path albedoTexture = albedoTextureHandle ? albedoTextureHandle.GetRelativePath() : std::filesystem::path();
		const std::filesystem::path normalTexture = normalTextureHandle ? normalTextureHandle.GetRelativePath() : std::filesystem::path();
		const std::filesystem::path materialTexture = materialTextureHandle ? materialTextureHandle.GetRelativePath() : std::filesystem::path();

		json["Albedo"] = albedoTexture;
		json["Normal"] = normalTexture;
		json["Material"] = materialTexture;

		return json;
	}
}