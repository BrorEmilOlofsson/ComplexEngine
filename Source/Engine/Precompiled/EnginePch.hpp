#pragma once

#ifdef _DEBUG
#include "Engine/Debugger/MemoryTracker/MemoryTracker.h"
#endif

#include <cassert>
#include <array>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <print>
#include <span>
#include <ranges>

#include <External/profiler.h>

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Math/Transform3.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"