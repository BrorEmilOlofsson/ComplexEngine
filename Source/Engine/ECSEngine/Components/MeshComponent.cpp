#include "Engine/Precompiled/EnginePch.hpp"
#include "MeshComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"
#include "Engine/Asset/AssetManager.hpp"
#include "Engine/Graphics/GraphicsConstants.hpp"

namespace Simple
{

	static void Initialize(MeshComponent& component, AssetManager& assetManager)
	{
		component.textures[TextureSlots::Albedo] = assetManager.GetTexture(GetPathByTextureType(eTextureType::Default_Albedo));
		component.textures[TextureSlots::Normal] = assetManager.GetTexture(GetPathByTextureType(eTextureType::Default_Normal));
		component.textures[TextureSlots::Material] = assetManager.GetTexture(GetPathByTextureType(eTextureType::Default_Material));

		component.mesh = assetManager.GetMesh(GetPathByPrimitiveMeshType(ePrimitiveShape::Cube));

		component.pixelShader = assetManager.GetPixelShader(GetPath(ePixelShaderType::LitDefault));
		component.vertexShader = assetManager.GetVertexShader(GetPath(eVertexShaderType::Default));
	}

	void Initialize(MeshComponent& meshComponent, const Blackboard& blackboard)
	{
		Initialize(meshComponent, blackboard.Get<Key_AssetManager>());
	}

	void RegisterMeshComponent()
	{
	}

	REGISTER_COMPONENT(MeshComponent);

	REGISTER_MEMBER(MeshComponent::mesh);
	REGISTER_MEMBER(MeshComponent::textures);
}