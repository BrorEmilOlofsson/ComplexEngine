#pragma once
#include "Engine/ECSEngine/Components/NameComponent.hpp"
#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Engine/ECSEngine/Components/TransformHierarchyComponent.hpp"
#include "Engine/ECSEngine/Components/EntityCompositionComponent.hpp"
#include "Engine/ECSEngine/Components/MeshComponent.hpp"
#include "Engine/ECSEngine/Components/ModelComponent.hpp"
#include "Engine/ECSEngine/Components/AnimationComponent.hpp"
#include "Engine/ECSEngine/Components/SkyBoxComponent.hpp"
#include "Engine/ECSEngine/Components/DirectionalLightComponent.hpp"
#include "Engine/ECSEngine/Components/PointLightComponent.hpp"
#include "Engine/ECSEngine/Components/ScriptComponent.hpp"
#include "Engine/ECSEngine/Components/CameraComponent.hpp"
#include "Engine/ECSEngine/Components/BloomComponent.hpp"
#include "Engine/ECSEngine/Components/Sprite2DComponent.hpp"
#include "Engine/ECSEngine/Components/Sprite3DComponent.hpp"
#include "Engine/ECSEngine/Components/UICanvasComponent.hpp"
#include "Engine/ECSEngine/Components/RotatingMovementComponent.hpp"
#include "Engine/ECSEngine/Components/ArrowComponent.hpp"

namespace Simple
{

	inline void RegisterEngineComponents()
	{
		RegisterNameComponent();
		RegisterTransformComponent();
		RegisterTransformHierarchyComponent();
		RegisterEntityCompositionComponent();
		RegisterMeshComponent();
		RegisterModelComponent();
		RegisterAnimationComponent();
		RegisterSkyBoxComponent();
		RegisterDirectionalLightComponent();
		RegisterPointLightComponent();
		RegisterScriptComponent();
		RegisterCameraComponent();
		RegisterBloomComponent();
		RegisterSprite2DComponent();
		RegisterSprite3DComponent();
		RegisterUICanvasComponent();
		RegisterRotatingMovementComponent();
		RegisterArrowComponent();
	}
}