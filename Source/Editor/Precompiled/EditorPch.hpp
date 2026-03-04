#pragma once

#ifdef _DEBUG
#include "Engine/Debugger/MemoryTracker/MemoryTracker.h"
#endif

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
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

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Math/Transform3.hpp"
#include "Engine/Math/Math.hpp"
#include "Engine/Input/InputState.hpp"

#include "Editor/Utility/EditorUtilities.hpp"
#include "Engine/Utility/ImGui/ImGuiUtility.hpp"
#include "Engine/Utility/EngineDirectories.hpp"
#include "Editor/Utility/EditorBlackboardKeys.hpp"

#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Engine/ECSEngine/Components/NameComponent.hpp"