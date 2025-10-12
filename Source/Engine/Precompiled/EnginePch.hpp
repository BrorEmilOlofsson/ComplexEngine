#pragma once

#ifdef _DEBUG
#include "Engine/Debugger/MemoryTracker/MemoryTracker.h"
#endif

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#define NOMINMAX
//#undef min
//#undef max

#include <array>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <External/profiler.h>

#include "Utility/Math/Vector2.hpp"
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/Vector4.hpp"
#include "Utility/Math/Matrix4x4.hpp"
#include "Utility/Math/Transform3.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"

#include "Engine/ECSEngine/Components/TransformComponent.hpp"