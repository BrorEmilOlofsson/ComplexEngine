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

#include <External/imgui/imgui.h>
#include <External/nlohmann/json.hpp>
#include <External/profiler.h>

#include "Utility/Math/Vector2.hpp"
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/Vector4.hpp"
#include "Utility/Math/Matrix4x4.hpp"
#include "Utility/Math/Transform3.hpp"
#include "Utility/Math/Math.hpp"
#include "Utility/Math/VectorMath.hpp"

#include "Utility/HashStuff.hpp"
#include "Utility/Shapes/Ray3.hpp"
#include "Utility/Shapes/Plane.hpp"
#include "Utility/Bounds.hpp"
#include "Utility/Algorithm.hpp"
#include "Utility/Input/InputState.hpp"

#include "Engine/Utility/BlackboardKeys.hpp"
#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Engine/ECSEngine/Components/NameComponent.hpp"
#include "Engine/ECSEngine/Components/MeshComponent.hpp"