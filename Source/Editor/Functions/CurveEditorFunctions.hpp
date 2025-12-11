#pragma once
#include "Utility/Math/Curve2.hpp"
#include "Editor/Command/Core/EditorCommandTracker.hpp"

namespace Simple
{

    void AddAnchorPointToCurve(Curve2f& curve, const Point2f& point, EditorCommandTracker& commandTracker);
}