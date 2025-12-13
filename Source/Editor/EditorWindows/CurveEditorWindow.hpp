#pragma once
#include "Utility/Math/CubicBezierCurve2.hpp"
#include "Utility/Blackboard.hpp"

namespace Simple
{

    void ShowCurveWindow(CubicBezierCurve2f& curve, bool& isActive, const Blackboard& blackboard);
}