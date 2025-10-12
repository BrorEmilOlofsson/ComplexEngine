#pragma once

#ifdef _DEBUG
#include "Engine/Debugger/MemoryTracker/MemoryTracker.h"
#endif

#define NOMINMAX

#include <vector>
#include <array>
#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>
#include <print>
#include <span>
#include <ranges>

#include <External/profiler.h>

#include "Utility/Math/Math.hpp"
#include "Utility/Math/Vector2.hpp"
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/Vector4.hpp"
#include "Utility/Math/Matrix4x4.hpp"
#include "Utility/Math/Transform3.hpp"
#include "Utility/Camera.hpp"