#pragma once

#ifdef _DEBUG
#include "Engine/Debugger/MemoryTracker/MemoryTracker.h"
#endif

#include <cassert>
#include <array>
#include <vector>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <span>
#include <ranges>

#include <External/imgui/imgui.h>
#include <External/nlohmann/json.hpp>
#include <External/profiler.h>

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Math/Transform3.hpp"
#include "Engine/Math/Math.hpp"
#include "Engine/Math/VectorMath.hpp"

#include "Engine/Math/Shapes/Ray3.hpp"
#include "Engine/Math/Shapes/Plane.hpp"
#include "Engine/Utility/Bounds.hpp"
#include "Engine/Utility/Algorithm.hpp"
#include "Engine/Input/InputState.hpp"

#include "Engine/Utility/BlackboardKeys.hpp"
#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Engine/ECSEngine/Components/NameComponent.hpp"
#include "Engine/ECSEngine/Components/MeshComponent.hpp"