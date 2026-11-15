#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include "Engine/Utility/Blackboard.hpp"
#include "Engine/Math/Transform3.hpp"
#include "Engine/ECS/EntitySerializationID.hpp"
#include "Engine/Utility/FixedString.hpp"

namespace CLX
{
	using Key_SceneRenderState = BlackboardKey<class RenderState&, FixedString("SceneRenderState")>;
	using Key_CurrentRenderState = BlackboardKey<class RenderState&, FixedString("CurrentRenderState")>;
	using Key_Navmesh = BlackboardKey<class Navmesh&, FixedString("Navmesh")>;
	using Key_SceneSettings = BlackboardKey<struct SceneSettings&, FixedString("SceneSettings")>;
	using Key_VariableName = BlackboardKey<std::string, FixedString("VariableName")>;
	using Key_CurrentCamera = BlackboardKey<class Camera&, FixedString("CurrentCamera")>;
	using Key_DeltaTime = BlackboardKey<float, FixedString("DeltaTime")>;
	using Key_ReferenceTransform = BlackboardKey<Transform, FixedString("ReferenceTransform")>;
	using Key_ShowUnitVectorInScene = BlackboardKey<bool&, FixedString("ShowUnitVectorInScene")>;
	using Key_IsPlaying = BlackboardKey<bool, FixedString("IsPlaying")>;
	using Key_GraphicsSettings = BlackboardKey<class GraphicsSettings&, FixedString("GraphicsSettings")>;
	using Key_WindowView = BlackboardKey<class WindowView, FixedString("WindowView")>;
	using Key_SceneManager = BlackboardKey<class SceneManager&, FixedString("SceneManager")>;
	using Key_AssetManager = BlackboardKey<class AssetManager&, FixedString("AssetManager")>;
	using Key_DataTypeRegistry = BlackboardKey<class DataTypeRegistry&, FixedString("DataTypeRegistry")>;
	using Key_InputState = BlackboardKey<const class InputState&, FixedString("InputState")>;
	using Key_OperatingSystem = BlackboardKey<class OperatingSystem&, FixedString("OperatingSystem")>;
	using Key_GraphicsFoundation = BlackboardKey<class GraphicsFoundation&, FixedString("GraphicsFoundation")>;
	using Key_AudioManager = BlackboardKey<class AudioManager&, FixedString("AudioManager")>;
	using Key_ECSRegistry = BlackboardKey<class ECSRegistry&, FixedString("ECSRegistry")>;
	using Key_ECSManager = BlackboardKey<class ECSManager&, FixedString("ECSManager")>;
	using Key_EntityIDGenerator = BlackboardKey<class EntitySerializationIDGenerator&, FixedString("EntityIDGenerator")>;
	using Key_OnEntitySelected = BlackboardKey<std::function<void(struct EntityID)>&, FixedString("OnEntitySelected")>;
	using Key_CurrentECS = BlackboardKey<class ECS&, FixedString("CurrentECS")>;
	using Key_CurrentECSConst = BlackboardKey<const class ECS&, FixedString("CurrentECSConst")>;
	using Key_CurrentPropertyPath = BlackboardKey<struct PropertyPath&, FixedString("CurrentPropertyPath")>;
	using Key_ActivePropertyPath = BlackboardKey<struct PropertyPath&, FixedString("ActivePropertyPath")>;
	using Key_EntitySerializationMap = BlackboardKey<const std::unordered_map<EntitySerializationID, EntityID>&, FixedString("EntitySerializationMap")>;
}