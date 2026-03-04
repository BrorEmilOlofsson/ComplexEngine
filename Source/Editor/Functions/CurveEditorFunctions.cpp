#include "Editor/Precompiled/EditorPch.hpp"
#include "CurveEditorFunctions.hpp"

namespace CLX
{

    void AddAnchorPointToCurve(CubicBezierCurve2f& curve, const Point2f& point, EditorCommandTracker& commandTracker)
    {
        const std::vector<Point2f> oldPoints = curve.GetPoints();
        curve.AddAnchorPoint(point);
        const std::vector<Point2f> newPoints = curve.GetPoints();

        struct AddAnchorPointData
        {
            CubicBezierCurve2f& curveRef;
            std::vector<Point2f> oldPoints;
            std::vector<Point2f> newPoints;
        };

        AddAnchorPointData data{ curve, oldPoints, newPoints };
        auto doCommand = [](const AddAnchorPointData& data)
            {
                data.curveRef.SetPoints(data.newPoints);
            };

        auto undoCommand = [](const AddAnchorPointData& data)
            {
                data.curveRef.SetPoints(data.oldPoints);
            };

        commandTracker.RegisterCommand(EditorCommand(data, doCommand, undoCommand, "Add Anchor Point To Curve"));
    }


    void SetCurvePointPosition(CubicBezierCurve2f& curve, uint32_t index, const Point2f& newPosition, EditorCommandTracker* commandTracker)
    {
        const Point2f oldPosition = curve.GetPoints()[index];
        SetCurvePointPosition(curve, index, newPosition, oldPosition, commandTracker);
    }

    void SetCurvePointPosition(CubicBezierCurve2f& curve, uint32_t index, const Point2f& newPosition, const Point2f& oldPosition, EditorCommandTracker* commandTracker)
    {
        struct MoveCurvePointData
        {
            CubicBezierCurve2f& curveRef;
            uint32_t pointIndex = 0;
            Point2f oldPosition;
            Point2f newPosition;
        };
        MoveCurvePointData data
        {
            curve,
            index,
            oldPosition,
            newPosition
        };
        auto doCommand = [](const MoveCurvePointData& data)
            {
                data.curveRef.SetPointAt(data.pointIndex, data.newPosition);
            };
        auto undoCommand = [](const MoveCurvePointData& data)
            {
                data.curveRef.SetPointAt(data.pointIndex, data.oldPosition);
            };

        if (commandTracker == nullptr)
        {
            doCommand(data);
        }
        else
        {
            commandTracker->ExecuteCommand(EditorCommand(data, doCommand, undoCommand, "Move Curve Point"));
        }
    }

}