#include "Editor/Precompiled/EditorPch.hpp"
#include "NavmeshEditor.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Editor/Functions/ECSEditorFunctions.hpp"
#include "Engine/Utility/DebugShapes.hpp"

namespace CLX
{

    void DebugFindPath(const Navmesh& navmesh, const InputState& input, RenderList& renderList, const Ray3f& mouseRay)
    {
        static Ray3f startRay;
        static Ray3f endRay;
        static std::optional<NavmeshPositionData> startHit;
        static std::optional<NavmeshPositionData> endHit;
        static std::vector<Point3f> path;
        if (input.IsKeyHeld(eInputKey::Shift) && input.IsKeyPressed(eInputKey::LMB))
        {
            startRay = mouseRay;
            startHit = navmesh.Raycast(startRay);
            if (startHit && endHit)
            {
                path = navmesh.FindPath(startHit->m3DPosition, endHit->m3DPosition);
            }
        }
        if (input.IsKeyHeld(eInputKey::Shift) && input.IsKeyPressed(eInputKey::RMB))
        {
            endRay = mouseRay;
            endHit = navmesh.Raycast(endRay);

            if (startHit && endHit)
            {
                path = navmesh.FindPath(startHit->m3DPosition, endHit->m3DPosition);
            }
        }
        if (startHit)
        {
            RenderSphere(Spheref::FromCenterAndRadius(startHit->m3DPosition, Radiusf(0.1f)), Colors::SkyBlue, renderList);
        }
        if (endHit)
        {
            RenderSphere(Spheref::FromCenterAndRadius(endHit->m3DPosition, Radiusf(0.1f)), Colors::Navy, renderList);
        }
        if (!path.empty())
        {
            if (startHit)
            {
                RenderLineSegment(startHit->m3DPosition, path[0], Colors::FrenchRose, renderList);
            }
        }
        if (path.size() >= 2)
        {
            for (std::size_t i = 0; i < path.size() - 1; ++i)
            {
                RenderLineSegment(path[i], path[i + 1], Colors::FrenchRose, renderList);
            }
        }
    }

    [[nodiscard]] NavmeshVertexIndex FindClosestVertex(const Navmesh& navmesh, const Ray3f& mouseRay)
    {
        NavmeshVertexIndex best = InvalidID<NavmeshVertexIndex>();
        float closestDistance = std::numeric_limits<float>::max();
        for (const auto& [index, vertex] : std::views::enumerate(navmesh.GetNavmeshData().m3DVertices))
        {
            const float distance = GetDistance(mouseRay, vertex);
            if (distance < closestDistance)
            {
                best = static_cast<NavmeshVertexIndex>(index);
                closestDistance = distance;
            }
        }

        return best;
    }

    Vector2f FSFS(const Navmesh& navmesh, NavmeshVertexIndex middleVertex, NavmeshVertexIndex leftVertex, NavmeshVertexIndex rightVertex)
    {
        const Point2f middlePos = ToPoint2XZ(navmesh.GetNavmeshData().m3DVertices[middleVertex]);
        const Point2f leftPos = ToPoint2XZ(navmesh.GetNavmeshData().m3DVertices[leftVertex]);
        const Point2f rightPos = ToPoint2XZ(navmesh.GetNavmeshData().m3DVertices[rightVertex]);
        const UnitVector2f leftToMiddle = GetUnitVector(leftPos, middlePos);
        const UnitVector2f middleToRight = GetUnitVector(rightPos, middlePos);

        const Vector2f added = leftToMiddle + middleToRight;
        Vector2f dir;
        if (LengthSquared(added) == 0.f)
        {
            dir = GetPerpendicularVector(leftToMiddle) * 1.f;
        }
        else
        {
            dir = ToNormalized(added);
        }

        return dir;
    }

    void NavmeshBuild(Navmesh& navmesh, const Ray3f& mouseRay, const InputState& input, const NavmeshEditorData&, RenderList& renderList, EditorCommandTracker& commandTracker)
    {
        commandTracker;
        if (input.IsKeyHeld(eInputKey::Ctrl))
        {
            auto result = navmesh.Raycast(mouseRay);
            if (!result)
            {
                NavmeshVertexIndex closestVertex = FindClosestVertex(navmesh, mouseRay);

                if (closestVertex != InvalidID<NavmeshVertexIndex>())
                {

                    std::vector<NavmeshVertexIndex> connectedVertices = navmesh.GetConnectedVertices(closestVertex);

                    auto v = connectedVertices | std::views::filter([&navmesh, closestVertex](const NavmeshVertexIndex connectedVertex)
                        {
                            auto connectedNodes = navmesh.GetConnectedNodes(closestVertex, connectedVertex);
                            if (connectedNodes.first == InvalidID<NavmeshNodeIndex>() && connectedNodes.second == InvalidID<NavmeshNodeIndex>())
                            {
                                throw std::runtime_error("Connected nodes not found for edge");
                            }

                            return !(connectedNodes.first != InvalidID<NavmeshNodeIndex>() && connectedNodes.second != InvalidID<NavmeshNodeIndex>());
                        }) | std::ranges::to<std::vector>();

                    
                    ASSERT_NEW(v.size() <= 2, "A vertex should not be connected to more than 2 other vertices in a well formed navmesh");

                    const Point3f closestPoint = navmesh.GetNavmeshData().m3DVertices[closestVertex];
                    std::optional<Point3f> p = GetPointAtY(mouseRay, closestPoint.y);

                    static Point3f p1;
                    static LineSegment3f l2;
                    if (p && input.IsKeyPressed(eInputKey::M))
                    {
                        p1 = *p;
                    }
                    renderList.AddSphere(DrawSphere{ .sphere = Spheref::FromCenterAndRadius(p1, Radiusf(0.2f)), .color = Colors::Cardinal });
                    renderList.AddLine(DrawLine{ .startPosition = l2.StartPoint(), .endPosition = l2.EndPoint(), .color = Colors::Cardinal });

                    NavmeshVertexIndex best = InvalidID<NavmeshVertexIndex>();
                    if (v.size() > 1)
                    {
                        if (p)
                        {
                            LineSegment2f l1 = LineSegment2f::FromPoints(ToPoint2XZ(closestPoint), ToPoint2XZ(closestPoint) + FSFS(navmesh, closestVertex, v[0], v.size() > 1 ? v[1] : v[0]));

                            if (IsOnNormalSide(ToPoint2XZ(*p), l1))
                            {
                                best = v[0];
                            }
                            else
                            {
                                best = v[1];
                            }
                            l2 = ToLineSegment3XZ(l1, closestPoint.y);
                        }
                    }
                    else
                    {
                        float closestDistance = std::numeric_limits<float>::max();

                        for (const NavmeshVertexIndex connectedVertex : v)
                        {
                            const float distance = GetDistance(mouseRay, navmesh.GetNavmeshData().m3DVertices[connectedVertex]);
                            if (distance < closestDistance)
                            {
                                best = connectedVertex;
                                closestDistance = distance;
                            }
                        }
                    }

                    if (best != InvalidID<NavmeshVertexIndex>())
                    {
                        if (p)
                        {
                            renderList.AddLine(DrawLine{ *p, navmesh.GetNavmeshData().m3DVertices[closestVertex], Colors::Yellow });
                            renderList.AddLine(DrawLine{ *p, navmesh.GetNavmeshData().m3DVertices[best], Colors::Yellow });
                        }
                    }
                }
            }
        }
    }

    void ShowNavmeshEditor(Scene& scene, const Camera& camera, const AABB2i& renderRect, const InputState& input, NavmeshEditorData& navmeshEditorData, EditorCommandTracker& commandTracker)
    {
        Navmesh& navmesh = scene.GetNavmesh();

        if (navmesh.GetNavmeshData().m3DVertices.empty())
        {
            return;
        }

        Ray3f mouseRay = scene.GetMouseRay();
        DebugFindPath(navmesh, input, scene.GetRenderState().GetRenderList(), mouseRay);
        NavmeshBuild(navmesh, mouseRay, input, navmeshEditorData, scene.GetRenderState().GetRenderList(), commandTracker);

        NavmeshVertexIndex& selectedVertexIndex = navmeshEditorData.selectedVertexIndex;

        if (input.IsKeyReleased(eInputKey::LMB))
        {
            NavmeshVertexIndex closestVertexIndex = FindClosestVertex(navmesh, mouseRay);
            if (closestVertexIndex != InvalidID<NavmeshVertexIndex>() 
                && closestVertexIndex != selectedVertexIndex
                && GetDistance(mouseRay, navmesh.GetNavmeshData().m3DVertices[closestVertexIndex]) < navmeshEditorData.vertexSelectDistance)
            {
                selectedVertexIndex = closestVertexIndex;
            }
        }

        if (selectedVertexIndex == InvalidID<NavmeshVertexIndex>())
        {
            return;
        }

        auto& vertexPos = navmesh.GetNavmeshData().m3DVertices[selectedVertexIndex];

        Transform transform = Transform::FromPosition(vertexPos);

        auto result = ShowImGuizmo(
            camera,
            transform,
            eTransformMode::World,
            eTransformOperation::Translate,
            renderRect,
            10,
            false,
            0.f
        );

        if (result && result->GetPosition() != vertexPos)
        {
            auto nodesWithThisVertex = navmesh.GetNodes()
                | std::views::enumerate
                | std::views::filter([selectedVertexIndex](const auto& pair) -> bool
                    {
                        const auto& [index, node] = pair;
                        return std::ranges::any_of(node.mVertexIndices, [selectedVertexIndex](const auto& vertex) -> bool
                            {
                                return vertex == selectedVertexIndex;
                            });
                    })
                | std::views::transform([selectedVertexIndex](const auto& pair)
                    {
                        const auto& [index, node] = pair;
                        return std::pair
                        {
                            static_cast<NavmeshNodeIndex>(index),
                            static_cast<NavmeshVertexIndex>(std::ranges::distance(std::ranges::begin(node.mVertexIndices), std::ranges::find(node.mVertexIndices, selectedVertexIndex)))
                        };
                    });

            auto faces = nodesWithThisVertex | std::views::transform([&navmesh, &result](const auto& pair)
                {
                    const auto& [nodeIndex, vertexIndexInNode] = pair;
                    Triangle3f face = navmesh.NodeToFace(nodeIndex);
                    face[vertexIndexInNode] = result->GetPosition();
                    return std::pair{ face, nodeIndex };
                }
            );

            NavmeshNodeIndex nodeIndexFacingDown = InvalidID<NavmeshNodeIndex>();

            const bool isAnyNormalDownwards = std::ranges::any_of(faces, [&nodeIndexFacingDown](const auto& pair) -> bool
                {
                    const auto& [face, nodeIndex] = pair;
                    if (ToPlane(face).GetNormal().Y() < 0)
                    {
                        nodeIndexFacingDown = nodeIndex;
                        return true;
                    }
                    return false;
                });

            const bool canMove = !isAnyNormalDownwards;
            if (canMove)
            {
                vertexPos = result->GetPosition();
            }
            else
            {
                NavmeshEdge edge;
                const auto& nodeFacingDown = navmesh.GetNode(nodeIndexFacingDown);

                if (selectedVertexIndex == nodeFacingDown.mVertexIndices[0])
                {
                    edge = { nodeFacingDown.mVertexIndices[1], nodeFacingDown.mVertexIndices[2] };
                }
                else if (selectedVertexIndex == nodeFacingDown.mVertexIndices[1])
                {
                    edge = { nodeFacingDown.mVertexIndices[0], nodeFacingDown.mVertexIndices[2] };
                }
                else
                {
                    edge = { nodeFacingDown.mVertexIndices[0], nodeFacingDown.mVertexIndices[1] };
                }


                auto lineSegment = navmesh.GetLineSegment2DFromEdge(edge);

                const auto projectedPoint = GetProjectedPoint(ToLine(lineSegment), ToPoint2XZ(result->GetPosition()));
                vertexPos = ToPoint3XZ(projectedPoint, vertexPos.y);
            }
        }
    }
}