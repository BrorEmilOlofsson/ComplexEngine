#include "Engine/Precompiled/EnginePch.hpp"
#include "ModelComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"
#include "Utility/Asset/AssetManager.hpp"
#include "Graphics/GraphicsConstants.hpp"

namespace Simple
{
	static void Initialize(ModelComponent& component, AssetManager& assetManager)
	{
		component.textures[TextureSlots::Albedo] = assetManager.GetTexture(GetPathByTextureType(eTextureType::Default_Albedo));
		component.textures[TextureSlots::Normal] = assetManager.GetTexture(GetPathByTextureType(eTextureType::Default_Normal));
		component.textures[TextureSlots::Material] = assetManager.GetTexture(GetPathByTextureType(eTextureType::Default_Material));

		component.model = assetManager.GetModel(GetPathByPrimitiveMeshType(ePrimitiveShape::Cube));

		component.pixelShader = assetManager.GetPixelShader(GetPath(ePixelShaderType::LitDefault));
		component.vertexShader = assetManager.GetVertexShader(GetPath(eVertexShaderType::Default));
	}

	void Initialize(ModelComponent& component, const Blackboard& blackboard)
	{
		Initialize(component, blackboard.Get<Key_AssetManager>());
	}

	void RegisterModelComponent()
	{
	}

	REGISTER_COMPONENT(ModelComponent);

	REGISTER_MEMBER(ModelComponent::model);
	REGISTER_MEMBER(ModelComponent::textures);
}