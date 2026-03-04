#include "Editor/Precompiled/EditorPch.hpp"
#include "CurveEditorWindow.hpp"
#include "Editor/Functions/CurveEditorFunctions.hpp"
#include "Engine/Math/Transform2.hpp"

namespace Simple
{

    void DrawBezierCurve(ImDrawList& drawList, const std::array<Point2f, 4>& controlPoints, Color color, float thickness)
    {
        drawList.AddBezierCubic(
            ToImVec2(controlPoints[0]),
            ToImVec2(controlPoints[1]),
            ToImVec2(controlPoints[2]),
            ToImVec2(controlPoints[3]),
            ToImGuiColor(color),
            thickness
        );
    }

    std::array<Point2f, 4> TransformPoints(const Transform2& windowTransform, const std::array<Point2f, 4>& points)
    {
        std::array<Point2f, 4> transformedPoints;
        std::ranges::transform(points, transformedPoints.begin(), [&](const Point2f& point)
            {
                Transform2 pointTransform;
                pointTransform.SetPosition(point);
                Transform2 worldTransform = pointTransform.ToWorld(windowTransform);
                return worldTransform.GetPosition();
            });

        return transformedPoints;
    }

    void UpdateControlPoints(CubicBezierCurve2f& curve, std::array<Point2f, 4> controlPoints, const Point2f windowOrigin, Point2f& pointPositionWhenDragStarted, bool includeFirst, uint32_t segmentIndex, EditorCommandTracker& commandTracker)
    {
        auto transformedControlPoints = TransformPoints(Transform2::FromPosition(windowOrigin), controlPoints);
        auto wasPointDragged = [&](const Point2f& p, uint32_t index) -> std::tuple<bool, bool, bool>
            {
                ImGui::SetCursorScreenPos({ p.x - 6, p.y - 6 });
                ImGui::InvisibleButton((std::string("##p") + std::to_string(index)).c_str(), { 12, 12 });
                return { ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left),  ImGui::IsItemActivated(), ImGui::IsItemDeactivated() };
            };


        const Vector2f mouseDelta = ToVector2(ImGui::GetIO().MouseDelta);

        const uint32_t start = includeFirst ? 0 : 1;
        for (uint32_t i = start; i < 4; ++i)
        {
            uint32_t index = curve.GetPointIndex(segmentIndex, i);
            auto [wasDragged, isActivated, isDeactivated] = wasPointDragged(transformedControlPoints[i], index);
            if (isActivated)
            {
                pointPositionWhenDragStarted = curve.GetPoints()[index];
            }
            if (wasDragged || isDeactivated)
            {
                EditorCommandTracker* commandTrackerPtr = wasDragged ? nullptr : &commandTracker;
                SetCurvePointPosition(curve, index, controlPoints[i] + mouseDelta, pointPositionWhenDragStarted, commandTrackerPtr);
            }
        }
    }


    void UpdateCurvePointPositions(CubicBezierCurve2f& curve, Point2f windowOrigin, Point2f& pointPositionWhenDragStarted, EditorCommandTracker& commandTracker)
    {
        for (auto i : std::views::iota(0uz, curve.GetNumSegments()))
        {
            auto controlPoints = curve.GetControlPointsAt(i);
            UpdateControlPoints(curve, controlPoints, windowOrigin, pointPositionWhenDragStarted, i == 0, static_cast<uint32_t>(i), commandTracker);
        }
    }

    void CheckForPointAdding(CubicBezierCurve2f& curve, Point2f windowOrigin, EditorCommandTracker& commandTracker)
    {
        if (ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            ImVec2 mousePos = ImGui::GetIO().MousePos;
            Point2f localPos = ToPoint2(mousePos) - ToVector2(ToImVec2(windowOrigin));
            AddAnchorPointToCurve(curve, localPos, commandTracker);
        }
    }

    void ShowCubicBezierCurve(ImDrawList& draw_list, Point2f windowOrigin, std::array<Point2f, 4> controlPoints, bool includeFirst)
    {

        // Transform control points to window space
        auto transformedControlPoints = TransformPoints(Transform2::FromPosition(windowOrigin), controlPoints);
        DrawBezierCurve(draw_list, transformedControlPoints, Colors::White, 2.f);

        // Optional: draw control points
        if (includeFirst)
        {
            draw_list.AddCircleFilled(ToImVec2(transformedControlPoints[0]), 4, IM_COL32(255, 0, 0, 255));
        }
        draw_list.AddCircleFilled(ToImVec2(transformedControlPoints[1]), 4, IM_COL32(0, 255, 0, 255));
        draw_list.AddCircleFilled(ToImVec2(transformedControlPoints[2]), 4, IM_COL32(0, 255, 0, 255));
        draw_list.AddCircleFilled(ToImVec2(transformedControlPoints[3]), 4, IM_COL32(255, 0, 0, 255));
    }

    void ShowCurve(const CubicBezierCurve2f& curve, ImDrawList& drawList, Point2f windowOrigin)
    {
        for (std::size_t i : std::views::iota(0uz, curve.GetNumSegments()))
        {
            auto controlPoints = curve.GetControlPointsAt(i);
            ShowCubicBezierCurve(drawList, windowOrigin, controlPoints, i == 0);
        }
    }

    void ShowCurveWindow(CubicBezierCurve2f& curve, bool& isWindowActive, const Blackboard& blackboard)
    {
        EditorCommandTracker& commandTracker = blackboard.Get<Key_CommandTracker>();
        curve;
        blackboard;
        if (ImGui::Begin("Curve Editor", &isWindowActive))
        {

            ImVec2 canvas_size(500, 500);
            ImGui::BeginChild("CurveCanvas", canvas_size, true);

            ImDrawList& drawList = *ImGui::GetWindowDrawList();
            Point2f origin = ToPoint2(ImGui::GetCursorScreenPos());
            ImVec2 size = ImGui::GetContentRegionAvail();

            if (curve.GetPoints().size() < 4)
            {
                curve.AddAnchorPoint({ 20.f, size.y - 20.f });
                curve.AddAnchorPoint({ size.x - 20.f, 20.f });
            }

            static Point2f pointPositionWhenDragStarted = Point2f::Zero();
            UpdateCurvePointPositions(curve, origin, pointPositionWhenDragStarted, commandTracker);
            CheckForPointAdding(curve, origin, commandTracker);
            ShowCurve(curve, drawList, origin);


            ImGui::EndChild();


        }

        ImGui::End();
    }
}