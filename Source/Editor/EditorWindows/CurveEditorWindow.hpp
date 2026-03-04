#pragma once
#include "Engine/Math/CubicBezierCurve2.hpp"
#include "Engine/Utility/Blackboard.hpp"

namespace Simple
{

    void ShowCurveWindow(CubicBezierCurve2f& curve, bool& isActive, const Blackboard& blackboard);
}