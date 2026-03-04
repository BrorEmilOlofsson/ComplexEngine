#pragma once
#include <string_view>
#include "Engine/Utility/Blackboard.hpp"

namespace Simple
{
	using Key_CommandTracker = BlackboardKey<class EditorCommandTracker&, "CommandTracker">;
	using Key_ECSBuffer = BlackboardKey<class ECS&, "ECSBuffer">;
	using Key_Editor = BlackboardKey<class Editor&, "Editor">;
	using Key_ImGuiStyleManager = BlackboardKey<class ImGuiStyleManager&, "ImGuiStyleManager">;
	using Key_FreeFlyCameraSettings = BlackboardKey<struct FreeFlyCameraSettings&, "FreeFlyCameraSettings">;
	using Key_EditorSceneSettings = BlackboardKey<struct EditorSceneSettings&, "EditorSceneSettings">;
	using Key_EntityCompositionPopUp = BlackboardKey<class EntityCompositionPopUp&, "EntityComposition">;
    using Key_NodeScriptingWindow = BlackboardKey<class NodeScriptingWindow&, "NodeScriptingWindow">;
}