#include "Engine/Precompiled/EnginePch.hpp"
#include "LoadDataFunctions.hpp"
#include "Utility/Win/WinConsole.hpp"
#include "Graphics/Shader/PixelShader.hpp"
#include "Graphics/Shader/VertexShader.hpp"
#include "Graphics/Texture/Texture.hpp"
#include "Graphics/Model/Skeleton.hpp"
#include "Graphics/Animation/Animation.hpp"
#include "Utility/Camera.hpp"
#include "Utility/Asset/AssetManager.hpp"
#include "Engine/Reflection/DataTypeRegistry.hpp"
#include "Graphics/GraphicsConstants.hpp"
#include <filesystem>

namespace Simple
{
	void LoadDataPtr(DataTypeID dataTypeID, void* dataPtr, const nlohmann::json& json, const Blackboard& blackboard)
	{
		blackboard.Get<Key_DataTypeRegistry>().LoadDataJSON(dataTypeID, dataPtr, json, blackboard);
	}
}

void FromJSON(bool& value, const nlohmann::json& json)
{
	value = json;
}

void FromJSON(char& value, const nlohmann::json& json)
{
	int v = static_cast<int>(value);
	FromJSON(v, json);
}

void FromJSON(int& value, const nlohmann::json& json)
{
	value = json;
}

void FromJSON(unsigned int& value, const nlohmann::json& json)
{
	value = json;
}

void FromJSON(float& value, const nlohmann::json& json)
{
	value = json;
}

namespace std
{
	void FromJSON(std::string& value, const nlohmann::json& json)
	{
		value = json;
	}
}

namespace Simple
{

	void FromJSON(Transform& transform, const nlohmann::json& json)
	{
		const auto& data = json;

		Point3f position;
		position.x = data["Position"]["x"];
		position.y = data["Position"]["y"];
		position.z = data["Position"]["z"];

		Rotatorf rotation;
		rotation.Pitch().Value() = data["Rotation"]["x"];
		rotation.Yaw().Value() = data["Rotation"]["y"];
		rotation.Roll().Value() = data["Rotation"]["z"];

		Vector3f scale;
		scale.x = data["Scale"]["x"];
		scale.y = data["Scale"]["y"];
		scale.z = data["Scale"]["z"];

		transform.SetScale(scale);
		transform.SetRotation(rotation);
		transform.SetPosition(position);
	}

	void FromJSON(Color& color, const nlohmann::json& json)
	{
		color.r = json.at("r");
		color.g = json.at("g");
		color.b = json.at("b");
		color.a = json.at("a");
	}

	void FromJSON(RGBColor& color, const nlohmann::json& json)
	{
		color.r = json.at("r");
		color.g = json.at("g");
		color.b = json.at("b");
	}

	void FromJSON(EntityID& entityID, const nlohmann::json& json)
	{
		::FromJSON(entityID.id, json);
	}

	void FromJSON(PointLight& pointLight, const nlohmann::json& json)
	{
		FromJSON(pointLight.color, json.at("Color"));
		pointLight.intensity = json.at("Intensity");
		pointLight.range = json.at("Range");
	}

	void FromJSON(DirectionalLight& directionalLight, const nlohmann::json& json)
	{
		FromJSON(directionalLight.direction, json.at("Direction"));
		FromJSON(directionalLight.color, json.at("Color"));
		directionalLight.intensity = json.at("Intensity");
	}

	void FromJSON(Camera& camera, const nlohmann::json& json)
	{
		const Vector2ui resolution = Vector2ui(1600, 900);

		Point3f position;
		position.x = json["Position"]["x"];
		position.y = json["Position"]["y"];
		position.z = json["Position"]["z"];

		camera.SetPosition(position);
		camera.SetHorizontalFOV(Radiansf(json["HorizontalFoV"]), resolution);
		camera.SetNearPlane(json["NearPlane"], resolution);
		camera.SetFarPlane(json["FarPlane"], resolution);
	}

	static void FromJSON(MeshAssetHandle& mesh, const nlohmann::json& json, AssetManager& assetManager)
	{
		const std::filesystem::path filePath = json;

		if (filePath.empty())
		{
			return;
		}
		const std::filesystem::path absolutePath = std::filesystem::absolute(SIMPLE_DIR_ASSETS / filePath);

		if (std::filesystem::exists(absolutePath) || absolutePath.string().find("Primitive") != std::string::npos)
		{
			mesh = assetManager.GetMesh(absolutePath);
			if (!mesh)
			{
				Console::Print("Mesh Error: Could not find mesh: " + absolutePath.string(), ConsoleTextColor::Red);
			}
		}
		else
		{
			const std::string text = "Mesh Error: Could not find file at " + absolutePath.string() + ". Primitive Mesh has replaced.";
			Console::Print(text, ConsoleTextColor::Red);
		}
	}

	void FromJSON(MeshAssetHandle& mesh, const nlohmann::json& json, const Blackboard& blackboard)
	{
		FromJSON(mesh, json, blackboard.Get<Key_AssetManager>());
	}

	static void FromJSON(ModelAssetHandle& modelAsset, const nlohmann::json& json, AssetManager& assetManager)
	{
		const std::filesystem::path filePath = json;

		if (filePath.empty())
		{
			return;
		}
		const std::filesystem::path absolutePath = std::filesystem::absolute(SIMPLE_DIR_ASSETS / filePath);

		if (std::filesystem::exists(absolutePath) || absolutePath.string().find("Primitive") != std::string::npos)
		{
			modelAsset = assetManager.GetModel(absolutePath);
			if (!modelAsset)
			{
				Console::Print("Mesh Error: Could not find mesh: " + absolutePath.string(), ConsoleTextColor::Red);
			}
		}
		else
		{
			const std::string text = "Mesh Error: Could not find file at " + absolutePath.string() + ". Primitive Mesh has replaced.";
			Console::Print(text, ConsoleTextColor::Red);
		}
	}

	void FromJSON(ModelAssetHandle& modelAsset, const nlohmann::json& json, const Blackboard& blackboard)
	{
		FromJSON(modelAsset, json, blackboard.Get<Key_AssetManager>());
	}

	static void FromJSON(AnimatedModelAssetHandle& animatedModelAsset, const nlohmann::json& json, AssetManager& assetManager)
	{
		const std::filesystem::path filePath = json;

		if (filePath.empty())
		{
			return;
		}
		const std::filesystem::path absolutePath = std::filesystem::absolute(SIMPLE_DIR_ASSETS / filePath);

		if (std::filesystem::exists(absolutePath) || absolutePath.string().find("Primitive") != std::string::npos)
		{
			animatedModelAsset = assetManager.GetAnimatedModel(absolutePath);
			if (!animatedModelAsset)
			{
				Console::Print("Mesh Error: Could not find mesh: " + absolutePath.string(), ConsoleTextColor::Red);
			}
		}
		else
		{
			const std::string text = "Mesh Error: Could not find file at " + absolutePath.string() + ". Primitive Mesh has replaced.";
			Console::Print(text, ConsoleTextColor::Red);
		}
	}

	void FromJSON(AnimatedModelAssetHandle& modelAsset, const nlohmann::json& json, const Blackboard& blackboard)
	{
		FromJSON(modelAsset, json, blackboard.Get<Key_AssetManager>());
	}

	static void FromJSON(TextureAssetHandle& texture, const nlohmann::json& json, AssetManager& assetManager)
	{
		const std::filesystem::path filePath = json;

		if (filePath.empty())
		{
			return;
		}

		const std::filesystem::path absolutePath = std::filesystem::absolute(SIMPLE_DIR_ASSETS / filePath);
		if (std::filesystem::exists(absolutePath))
		{
			texture = assetManager.GetTexture(absolutePath);
			if (!texture)
			{
				Console::Print("Texture Error: Could not find texture: " + absolutePath.string(), ConsoleTextColor::Red);
			}
		}
		else
		{
			texture = assetManager.GetTexture(GetPathByTextureType(eTextureType::Default_Albedo));
			const std::string text = "Texture Error: Could not find file at " + absolutePath.string() + ". Default Texture has replaced.";
			Console::Print(text, ConsoleTextColor::Red);
		}
	}

	void FromJSON(TextureAssetHandle& textureAsset, const nlohmann::json& json, const Blackboard& blackboard)
	{
		FromJSON(textureAsset, json, blackboard.Get<Key_AssetManager>());
	}

	static void FromJSON(AnimationAssetHandle& animationAsset, const nlohmann::json& json, AssetManager& assetManager)
	{
		const std::string animationRelativeFilePath = json;

		if (animationRelativeFilePath.empty())
		{
			return;
		}

		animationAsset = assetManager.GetAnimation(animationRelativeFilePath);
	}

	void FromJSON(AnimationAssetHandle& animationAsset, const nlohmann::json& json, const Blackboard& blackboard)
	{
		return FromJSON(animationAsset, json, blackboard.Get<Key_AssetManager>());
	}

	static void FromJSON(PixelShaderAssetHandle& shader, const nlohmann::json& json, AssetManager& assetManager)
	{
		std::filesystem::path pixelShaderPath = std::filesystem::path(std::string(json["PixelShader"]));

		if (!pixelShaderPath.empty())
		{
			shader = assetManager.GetPixelShader(pixelShaderPath);
		}
	}

	void FromJSON(PixelShaderAssetHandle& shader, const nlohmann::json& json, const Blackboard& blackboard)
	{
		FromJSON(shader, json, blackboard.Get<Key_AssetManager>());
	}

	static void FromJSON(VertexShaderAssetHandle& shader, const nlohmann::json& json, AssetManager& assetManager)
	{
		std::filesystem::path vertexShaderPath = std::filesystem::path(std::string(json["VertexShader"]));

		if (!vertexShaderPath.empty())
		{
			shader = assetManager.GetVertexShader(vertexShaderPath);
		}
	}

	void FromJSON(VertexShaderAssetHandle& shader, const nlohmann::json& json, const Blackboard& blackboard)
	{
		FromJSON(shader, json, blackboard.Get<Key_AssetManager>());
	}

	constexpr std::string_view GetNameBySlot(const std::size_t slot)
	{
		switch (slot)
		{
		case TextureSlots::Albedo:
			return "Albedo";
		case TextureSlots::Normal:
			return "Normal";
		case TextureSlots::Material:
			return "Material";
		default:
			throw std::out_of_range("Invalid texture slot index");
		}
	}

	static void CustomFromJSON(std::array<TextureAssetHandle, 3>& textureAssets, const nlohmann::json& json, AssetManager& assetManager)
	{

		auto load = [&textureAssets, &assetManager](const std::filesystem::path& texturePath, const std::size_t index) -> std::tuple<bool, std::filesystem::path>
			{
				const std::filesystem::path absolutePath = std::filesystem::absolute(SIMPLE_DIR_ASSETS / texturePath);
				if (!absolutePath.empty() && std::filesystem::exists(absolutePath))
				{
					const TextureAssetHandle handle = assetManager.GetTexture(absolutePath);
					if (!handle)
					{
						return { false, absolutePath };
					}
					textureAssets[index] = handle;
					return { true, "" };
				}

				return { false, absolutePath };
			};

		for (std::size_t i = 0; i < textureAssets.size(); ++i)
		{
			auto [isValidFile, loadFailureText] = load(json[GetNameBySlot(i)], i);

			if (!isValidFile)
			{
				textureAssets[i] = assetManager.GetTexture(GetPathByTextureType(eTextureType::Default_Albedo));
				const std::string text = "Texture Error: Could not find file at " + loadFailureText.string() + ". Default Texture has replaced.";
				Console::Print(text, ConsoleTextColor::Red);
			}
		}
	}

	void CustomFromJSON(std::array<TextureAssetHandle, 3>& textureAssets, const nlohmann::json& json, const Blackboard& blackboard)
	{
		CustomFromJSON(textureAssets, json, blackboard.Get<Key_AssetManager>());
	}
}