#pragma once

#ifdef _DEBUG
#include "Engine/Debugger/MemoryTracker/MemoryTracker.h"
#endif

#include <array>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <print>

#include <External/profiler.h>

#include "Utility/Math/Vector2.hpp"
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/Vector4.hpp"
#include "Utility/Math/Matrix4x4.hpp"
#include "Utility/Math/Transform3.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"

#include "Engine/ECSEngine/Components/TransformComponent.hpp"