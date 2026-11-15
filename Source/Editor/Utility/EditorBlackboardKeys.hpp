#pragma once
#include <string_view>
#include <map>
#include <vector>
#include <any>
#include "Engine/Utility/Blackboard.hpp"

namespace CLX
{

	using Key_CommandTracker = BlackboardKey<class EditorCommandTracker&, FixedString("CommandTracker")>;
	using Key_ECSBuffer = BlackboardKey<class ECS&, FixedString("ECSBuffer")>;
	using Key_Editor = BlackboardKey<class Editor&, FixedString("Editor")>;
	using Key_EditorWindowManager = BlackboardKey<class EditorWindowManager&, FixedString("EditorWindowManager")>;
	using Key_ImGuiStyleManager = BlackboardKey<class ImGuiStyleManager&, FixedString("ImGuiStyleManager")>;
	using Key_FreeFlyCameraSettings = BlackboardKey<struct FreeFlyCameraSettings&, FixedString("FreeFlyCameraSettings")>;
	using Key_EditorSceneSettings = BlackboardKey<struct EditorSceneSettings&, FixedString("EditorSceneSettings")>;
    using Key_NodeScriptingWindow = BlackboardKey<class NodeScriptingWindow&, FixedString("NodeScriptingWindow")>;
    using Key_EntityCompositionInstantiationManager = BlackboardKey<class EntityCompositionInstantiationManager&, FixedString("EntityCompositionInstantiationManager")>;
    using Key_CurrentCopiedComponent = BlackboardKey<std::any&, FixedString("CurrentCopiedComponent")>;
}