#include "Editor/Precompiled/EditorPch.hpp"
#include "CurveEditorFunctions.hpp"

namespace Simple
{

    void AddAnchorPointToCurve(Curve2f& curve, const Point2f& point, EditorCommandTracker& commandTracker)
    {
        const std::vector<Point2f> oldPoints = curve.GetPoints();
        curve.AddAnchorPoint(point);
        const std::vector<Point2f> newPoints = curve.GetPoints();

        struct AddAnchorPointData
        {
            Curve2f& curveRef;
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

}