#pragma once
#include "Engine/ECSEngine/Components/NameComponent.hpp"
#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Engine/ECSEngine/Components/TransformHierarchyComponent.hpp"
#include "Engine/ECSEngine/Components/EntityCompositionInstantiationComponent.hpp"
#include "Engine/ECSEngine/Components/MeshComponent.hpp"
#include "Engine/ECSEngine/Components/ModelComponent.hpp"
#include "Engine/ECSEngine/Components/AnimatedModelComponent.hpp"
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
#include "Engine/ECSEngine/Components/CylinderComponent.hpp"
#include "Engine/ECSEngine/Components/TriggerComponent.hpp"

namespace CLX
{

	inline void RegisterEngineComponents()
	{
		RegisterNameComponent();
		RegisterTransformComponent();
		RegisterTransformHierarchyComponent();
		RegisterEntityCompositionInstantiationComponent();
		RegisterMeshComponent();
		RegisterModelComponent();
		RegisterAnimatedModelComponent();
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
        RegisterCylinderComponent();
		RegisterTriggerComponent();
	}
}