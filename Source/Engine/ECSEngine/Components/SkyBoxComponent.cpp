#include "Engine/Precompiled/EnginePch.hpp"
#include "SkyBoxComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"
#include "Engine/Asset/AssetManager.hpp"

namespace Simple
{
	
	static void Initialize(SkyBoxComponent& component, AssetManager& assetManager)
	{
		component.mesh = assetManager.GetMesh(GetPathByPrimitiveMeshType(ePrimitiveShape::SkyBox));
		component.texture = assetManager.GetTexture(GetPath(eSkyBox::AutumnForest));
		component.pixelShader = assetManager.GetPixelShader(GetPath(ePixelShaderType::LitDefault));
		component.vertexShader = assetManager.GetVertexShader(GetPath(eVertexShaderType::Default));
	}

	void Initialize(SkyBoxComponent& component, const Blackboard& blackboard)
	{
		Initialize(component, blackboard.Get<Key_AssetManager>());
	}

	void RegisterSkyBoxComponent()
	{
	}


	REGISTER_COMPONENT(SkyBoxComponent);

	REGISTER_MEMBER(SkyBoxComponent::mesh);
	REGISTER_MEMBER(SkyBoxComponent::texture);
	REGISTER_MEMBER(SkyBoxComponent::useSkyBoxFlooring);
	REGISTER_MEMBER(SkyBoxComponent::useUnityShader);
}