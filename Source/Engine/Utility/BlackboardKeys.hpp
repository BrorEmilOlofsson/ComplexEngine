#pragma once
#include "Engine/Utility/Blackboard.hpp"
#include "Engine/Math/Transform3.hpp"
#include <string>
#include <Engine/ECS/ECSRegistry.hpp>

namespace Simple
{
	using Key_SceneRenderState = BlackboardKey<class RenderState&, "SceneRenderState">;
	using Key_CurrentRenderState = BlackboardKey<class RenderState&, "CurrentRenderState">;
	using Key_Navmesh = BlackboardKey<class Navmesh&, "Navmesh">;
	using Key_SceneSettings = BlackboardKey<struct SceneSettings&, "SceneSettings">;
	using Key_VariableName = BlackboardKey<std::string, "VariableName">;
	using Key_CurrentCamera = BlackboardKey<class Camera&, "CurrentCamera">;
	using Key_DeltaTime = BlackboardKey<float, "DeltaTime">;
	using Key_ReferenceTransform = BlackboardKey<Transform, "ReferenceTransform">;
	using Key_ShowUnitVectorInScene = BlackboardKey<bool&, "ShowUnitVectorInScene">;
	using Key_IsPlaying = BlackboardKey<bool, "IsPlaying">;
	using Key_GraphicsSettings = BlackboardKey<class GraphicsSettings&, "GraphicsSettings">;
	using Key_WindowView = BlackboardKey<class WindowView, "WindowView">;
	using Key_SceneManager = BlackboardKey<class SceneManager&, "SceneManager">;
	using Key_AssetManager = BlackboardKey<class AssetManager&, "AssetManager">;
	using Key_DataTypeRegistry = BlackboardKey<class DataTypeRegistry&, "DataTypeRegistry">;
	using Key_InputState = BlackboardKey<const class InputState&, "InputState">;
	using Key_OperatingSystem = BlackboardKey<class OperatingSystem&, "OperatingSystem">;
	using Key_AudioManager = BlackboardKey<class AudioManager&, "AudioManager">;
    using Key_ECSRegistry = BlackboardKey<class ECSRegistry&, "ECSRegistry">;
}