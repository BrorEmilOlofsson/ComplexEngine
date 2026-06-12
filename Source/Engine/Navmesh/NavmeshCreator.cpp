#include "Engine/Precompiled/EnginePch.hpp"
#include "NavmeshCreator.hpp"
#include "NavmeshData.hpp"
#include "Navmesh.hpp"

namespace CLX
{

    std::vector<NavmeshNode> CreateNodes(std::span<const NavmeshNodeIndex> indices, std::span<const Point2f> vertices2D, std::span<const Point3f> vertices3D)
    {
        PROFILER_FUNCTION(profiler::colors::Grey300);
        std::vector<NavmeshNode> nodes;
        nodes.reserve(indices.size() / Vertices_In_Triangle);
        for (NavmeshNodeIndex nodeIndex = 0; nodeIndex < indices.size(); nodeIndex += Vertices_In_Triangle)
        {
            NavmeshNode& newNode = nodes.emplace_back();

            newNode.mVertexIndices[0] = indices[nodeIndex + 0ull];
            newNode.mVertexIndices[1] = indices[nodeIndex + 1ull];
            newNode.mVertexIndices[2] = indices[nodeIndex + 2ull];

            // Calculate center
            const Point2f& pos1 = vertices2D[newNode.mVertexIndices[0]];
            const Point2f& pos2 = vertices2D[newNode.mVertexIndices[1]];
            const Point2f& pos3 = vertices2D[newNode.mVertexIndices[2]];
            newNode.mCenter = AveragePoint(pos1, pos2, pos3);

            // Calculate center
            const Point3f& pos13D = vertices3D[newNode.mVertexIndices[0]];
            const Point3f& pos23D = vertices3D[newNode.mVertexIndices[1]];
            const Point3f& pos33D = vertices3D[newNode.mVertexIndices[2]];
            newNode.mCenter3D = AveragePoint(pos13D, pos23D, pos33D);

            newNode.mPlane = Planef::FromPoints(newNode.mCenter3D, pos23D, pos33D);

            ASSERT_NEW(newNode.mPlane.GetNormal().Y() >= 0, "Normal of navmesh face points down");

        }
        return nodes;
    }

    constexpr std::size_t GetSharedVertexCount(const NavmeshNode& node1, const NavmeshNode& node2) noexcept
    {
        return std::ranges::count_if(node1.mVertexIndices, [&](auto idx1)
            {
                return std::ranges::any_of(node2.mVertexIndices, [&](auto idx2)
                    {
                        return idx1 == idx2;
                    });
            });
    }

    void CalculateConnections(std::span<NavmeshNode> nodes)
    {
        PROFILER_FUNCTION(profiler::colors::Grey300);
        const size_t size = nodes.size();
        for (NavmeshNodeIndex currentNodeIndex = 0; currentNodeIndex < size; ++currentNodeIndex)
        {
            NavmeshNode& node = nodes[currentNodeIndex];
            NavmeshNodeIndex startNode = 0;
            for (NavmeshNodeIndex& nodeConnectionIndex : node.mConnections)
            {
                NavmeshNodeIndex connectionIndex = GetInvalidIndex<NavmeshNodeIndex>();

                for (NavmeshNodeIndex i = startNode; i < size; ++i)
                {
                    if (currentNodeIndex == i) continue;

                    const NavmeshNode& connection = nodes[i];

                    if (GetSharedVertexCount(node, connection) == 2)
                    {
                        connectionIndex = i;
                        startNode = i + 1;
                        break;
                    }
                }
                nodeConnectionIndex = connectionIndex;
            }
        }
    }

    [[nodiscard]] static std::vector<NavmeshWall> CreateWalls(std::span<const NavmeshNode> nodes)
    {
        PROFILER_FUNCTION(profiler::colors::Grey300);
        std::vector<NavmeshWall> walls;
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            for (NavmeshVertexIndex j = 0; j < Vertices_In_Triangle; ++j)
            {
                const NavmeshVertexIndex otherIndex = (j + 1) % Vertices_In_Triangle;

                const NavmeshVertexIndex a = nodes[i].mVertexIndices[j];
                const NavmeshVertexIndex b = nodes[i].mVertexIndices[otherIndex];


                bool isWall = true;

                for (size_t k = 0; k < Vertices_In_Triangle && isWall; ++k)
                {
                    const NavmeshNodeIndex connectionNodeIndex = nodes[i].mConnections[k];

                    for (NavmeshVertexIndex l = 0; l < Vertices_In_Triangle; ++l)
                    {
                        const NavmeshVertexIndex otherIndex2 = (l + 1) % Vertices_In_Triangle;

                        if (connectionNodeIndex == GetInvalidIndex<NavmeshVertexIndex>())
                        {
                            continue;
                        }

                        const NavmeshVertexIndex c = nodes[connectionNodeIndex].mVertexIndices[l];
                        const NavmeshVertexIndex d = nodes[connectionNodeIndex].mVertexIndices[otherIndex2];

                        if (a == d && b == c)
                        {
                            isWall = false;
                            break;
                        }
                    }
                }

                if (isWall)
                {
                    NavmeshWall wall(a, b);

                    walls.push_back(wall);
                }
            }
        }

        for (size_t i = 0; i < walls.size(); ++i)
        {
            bool connectionExists = false;
            for (size_t j = 0; j < walls.size(); ++j)
            {
                if (walls[i].first == walls[j].second)
                {
                    connectionExists = true;
                    break;
                }
            }
            if (!connectionExists)
            {
                assert(false && "There are walls inside the navmesh!");
            }
        }

        return walls;
    }

    [[nodiscard]] std::vector<UnitVector3f> CalculatePathOffsets(std::span<const NavmeshWall> walls, std::span<const Point2f> vertices)
    {
        PROFILER_FUNCTION(profiler::colors::Grey300);
        std::vector<UnitVector3f> pathOffsets(vertices.size());

        for (const NavmeshWall& wall1 : walls)
        {
            const size_t currentVertexIndex = wall1.first;
            const Point2f& leftPos = vertices[wall1.second];
            const Point2f& middlePos = vertices[currentVertexIndex];
            std::optional<Point2f> rightPos = std::nullopt;
            for (const NavmeshWall& wall2 : walls)
            {
                if (wall1.first == wall2.second)
                {
                    rightPos = vertices[wall2.first];
                }
            }
            if (!rightPos)
            {
                assert(false && "Navmesh is broken, probably!");
                return {};
            }

            const UnitVector2f middleToLeft = GetUnitVector(middlePos, leftPos);
            const UnitVector2f middleToRight = GetUnitVector(middlePos, *rightPos);

            const Vector2f added = middleToLeft + middleToRight;
            Vector2f dir;
            if (LengthSquared(added) == 0.f)
            {
                dir = GetPerpendicularVector(middleToLeft) * 1.f;
            }
            else
            {
                dir = ToNormalized(-added);
            }

            const Vector3f dir3D = ToVector3XZ(dir);

            pathOffsets[currentVertexIndex] = UnitVector3f(dir3D);
        }

        return pathOffsets;
    }

    [[nodiscard]] AABB2f CalculateDimensions(std::span<const Point2f> vertices)
    {
        if (!vertices.empty())
        {
            AABB2f dimensions = CreateAABB2FromPoints(vertices);
            dimensions.SetMin(dimensions.GetMin() + Vector2f(-Navmesh::Dimensions_Offset, -Navmesh::Dimensions_Offset));
            dimensions.SetMax(dimensions.GetMax() + Vector2f(Navmesh::Dimensions_Offset, Navmesh::Dimensions_Offset));
            return dimensions;
        }
        else
        {
            AABB2f dimensions;
            dimensions.SetMin(dimensions.GetMin() + Vector2f(-Navmesh::Dimensions_Offset, -Navmesh::Dimensions_Offset));
            dimensions.SetMax(dimensions.GetMax() + Vector2f(Navmesh::Dimensions_Offset, Navmesh::Dimensions_Offset));
            return dimensions;
        }
    }

    [[nodiscard]] Grid2<NavmeshNodeIndex> CreateNodeGrid(std::span<const NavmeshNode> nodes, std::span<const Point2f> vertices, AABB2f dimensions)
    {
        PROFILER_FUNCTION(profiler::colors::Grey300);
        const size_t nodeSize = nodes.size();
        const Vector2f extent = dimensions.GetExtent();
        const unsigned int numTilesPerDimension = static_cast<unsigned int>(Sqrt(static_cast<float>(nodeSize))) + 1;
        const Vector2u gridSize(numTilesPerDimension, numTilesPerDimension);
        const Vector2f cellSize(extent.x / gridSize.x, extent.y / gridSize.y);
        const Point2f startPos = dimensions.GetMin();
        Grid2<NavmeshNodeIndex> nodeGrid(gridSize, cellSize, startPos);

        for (NavmeshNodeIndex nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
        {
            std::vector<Point2f> points;
            for (const NavmeshVertexIndex vertexIndex : nodes[nodeIndex].mVertexIndices)
            {
                const Point2f& vertexPos = vertices[vertexIndex];
                points.push_back(vertexPos);
            }

            const AABB2f nodeDimensions = CreateAABB2FromPoints(std::span<const Point2f>(points));

            nodeGrid.AddObjectToCellsInsideAABB(nodeIndex, nodeDimensions);
        }
        return nodeGrid;
    }

    [[nodiscard]] Grid2<NavmeshVertexIndex> CreateVertexGrid(std::span<const Point2f> vertices, AABB2f dimensions)
    {
        PROFILER_FUNCTION(profiler::colors::Grey300);
        const size_t vertexSize = vertices.size();
        const Vector2f diffMaxMin = dimensions.GetMax() - dimensions.GetMin();
        const unsigned int numTilesPerDimension = static_cast<unsigned int>(sqrt(vertexSize)) + 1;
        const Vector2u gridSize(numTilesPerDimension, numTilesPerDimension);
        const Vector2f cellSize(diffMaxMin.x / gridSize.x, diffMaxMin.y / gridSize.y);
        const Point2f startPos = dimensions.GetMin();
        Grid2<NavmeshVertexIndex> vertexGrid(gridSize, cellSize, startPos);

        for (const auto& [vertexIndex, vertexPos] : std::views::enumerate(vertices))
        {
            auto cell = vertexGrid.GetCellByPosition(vertexPos);
            if (!cell)
            {
                assert(false && "Vertex is outside of navmesh dimensions");
                continue;
            }
            cell->Add(static_cast<NavmeshVertexIndex>(vertexIndex));
        }
        return vertexGrid;
    }

    [[nodiscard]] Grid2<NavmeshWallIndex> CreateWallGrid(std::span<const NavmeshWall> walls, std::span<const Point2f> vertices, AABB2f dimensions)
    {
        PROFILER_FUNCTION(profiler::colors::Grey300);
        AABB2f scaledDimensions = ScaleAABB(dimensions, 1.5f);

        const size_t wallSize = walls.size();
        const Vector2f diffMaxMin = dimensions.GetExtent();
        const unsigned int numTilesPerDimension = static_cast<unsigned int>(sqrt(wallSize)) + 10u;
        const Vector2u gridSize(numTilesPerDimension, numTilesPerDimension);
        const Vector2f cellSize(diffMaxMin.x / gridSize.x, diffMaxMin.y / gridSize.y);
        const Point2f startPos = dimensions.GetMin();
        Grid2<NavmeshWallIndex> wallGrid(gridSize, cellSize, startPos);

        for (NavmeshWallIndex wallIndex = 0; wallIndex < walls.size(); ++wallIndex)
        {
            const NavmeshWall& wall = walls[wallIndex];
            std::vector<Point2f> points =
            {
                vertices[wall.first],
                vertices[wall.second]
            };

            const AABB2f wallDimensions = CreateAABB2FromPoints(std::span<const Point2f>(points));

            wallGrid.AddObjectToCellsInsideAABB(wallIndex, wallDimensions);
        }
        return wallGrid;
    }

    NavmeshCreatorResult NavmeshCreator::CreateNavmesh(NavmeshData& navmeshData)
    {
        PROFILER_FUNCTION(profiler::colors::Grey300);
        std::vector<NavmeshNode> nodes = CreateNodes(navmeshData.mIndices, navmeshData.mVertices, navmeshData.m3DVertices);
        CalculateConnections(nodes);
        std::vector<NavmeshWall> walls = CreateWalls(nodes);
        std::vector<UnitVector3f> pathOffsets = CalculatePathOffsets(walls, navmeshData.mVertices);
        AABB2f dimensions = CalculateDimensions(navmeshData.mVertices);
        Grid2<NavmeshNodeIndex> nodeGrid = CreateNodeGrid(nodes, navmeshData.mVertices, dimensions);
        Grid2<NavmeshVertexIndex> vertexGrid = CreateVertexGrid(navmeshData.mVertices, dimensions);
        Grid2<NavmeshWallIndex> wallGrid = CreateWallGrid(walls, navmeshData.mVertices, dimensions);
        return { std::move(nodes), std::move(walls), std::move(nodeGrid), std::move(vertexGrid), std::move(wallGrid), dimensions };
    }
}
