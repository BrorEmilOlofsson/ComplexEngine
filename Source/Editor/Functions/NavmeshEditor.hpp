#pragma once
#include "Engine/Math/Shapes/AABB2.hpp"

namespace CLX
{

    class Scene;
    class Camera;
    class InputState;

    void ShowNavmeshEditor(Scene& scene, const Camera& camera, const AABB2i& renderRect, const InputState& input);
}