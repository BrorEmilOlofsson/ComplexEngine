#pragma once
#include <string_view>
#include <map>
#include <vector>
#include "Engine/Utility/Blackboard.hpp"

namespace CLX
{

	using Key_CommandTracker = BlackboardKey<class EditorCommandTracker&, "CommandTracker">;
	using Key_ECSBuffer = BlackboardKey<class ECS&, "ECSBuffer">;
	using Key_Editor = BlackboardKey<class Editor&, "Editor">;
	using Key_EditorWindowManager = BlackboardKey<class EditorWindowManager&, "EditorWindowManager">;
	using Key_ImGuiStyleManager = BlackboardKey<class ImGuiStyleManager&, "ImGuiStyleManager">;
	using Key_FreeFlyCameraSettings = BlackboardKey<struct FreeFlyCameraSettings&, "FreeFlyCameraSettings">;
	using Key_EditorSceneSettings = BlackboardKey<struct EditorSceneSettings&, "EditorSceneSettings">;
    using Key_NodeScriptingWindow = BlackboardKey<class NodeScriptingWindow&, "NodeScriptingWindow">;
    using Key_EntityCompositionInstantiationManager = BlackboardKey<class EntityCompositionInstantiationManager&, "EntityCompositionInstantiationManager">;
    using Key_CurrentCopiedComponent = BlackboardKey<struct JsonAny&, "CurrentCopiedComponent">;
}