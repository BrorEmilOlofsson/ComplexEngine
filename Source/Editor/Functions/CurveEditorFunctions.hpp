#pragma once
#include "Engine/Math/CubicBezierCurve2.hpp"
#include "Editor/Command/Core/EditorCommandTracker.hpp"

namespace Simple
{

    void AddAnchorPointToCurve(CubicBezierCurve2f& curve, const Point2f& point, EditorCommandTracker& commandTracker);
    void SetCurvePointPosition(CubicBezierCurve2f& curve, uint32_t index, const Point2f& newPosition, EditorCommandTracker* commandTracker);
    void SetCurvePointPosition(CubicBezierCurve2f& curve, uint32_t index, const Point2f& newPosition, const Point2f& oldPosition, EditorCommandTracker* commandTracker);
}