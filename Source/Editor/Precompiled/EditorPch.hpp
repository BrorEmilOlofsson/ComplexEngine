#pragma once

#ifdef _DEBUG
#include "Engine/Debugger/MemoryTracker/MemoryTracker.h"
#endif

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <span>
#include <optional>
#include <expected>
#include <print>

#include <External/imgui/imgui.h>
#include <External/imgui/imnodes/imnodes.h>
#include <External/imgui/imguizmo/imguizmo.h>
#include <External/nlohmann/json.hpp>
#include <External/profiler.h>

#include "Utility/Math/Vector2.hpp"
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/Vector4.hpp"
#include "Utility/Math/Matrix4x4.hpp"
#include "Utility/Math/Transform3.hpp"
#include "Utility/Math/Math.hpp"
#include "Utility/Input/InputState.hpp"

#include "Editor/Utility/EditorUtilities.hpp"
#include "Utility/ImGui/ImGuiUtility.hpp"
#include "Utility/EngineDirectories.hpp"
#include "Editor/Utility/EditorBlackboardKeys.hpp"

#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Engine/ECSEngine/Components/NameComponent.hpp"