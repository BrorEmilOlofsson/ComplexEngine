#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/ShapeMath.hpp"

using namespace Simple;

TEST_CASE("ShapeMath::ToLine (LineSegment2 -> Line2)", "[ShapeMath]")
{
    constexpr LineSegment2f segment = LineSegment2f::FromPoints(Point2f(0.0f, 0.0f), Point2f(1.0f, 1.0f));
    const Line2f line = ToLine(segment);

    REQUIRE(line.GetPoint() == Point2f(0.0f, 0.0f));
    REQUIRE(NearlyEqual(line.GetDirection(), UnitVector2f(1.0f, 1.0f)));
}

TEST_CASE("ShapeMath::ToLine (LineSegment3 -> Line3)", "[ShapeMath]")
{
    constexpr LineSegment3f segment = LineSegment3f::FromPoints(Point3f(0.0f, 0.0f, 0.0f), Point3f(1.0f, 1.0f, 1.0f));
    const Line3f line = ToLine(segment);
    REQUIRE(line.GetPoint() == Point3f(0.0f, 0.0f, 0.0f));
    REQUIRE(NearlyEqual(line.GetDirection(), UnitVector3f(1.0f, 1.0f, 1.0f)));
}

TEST_CASE("ShapeMath::ToLineSegment (Ray2 -> LineSegment2)", "[ShapeMath]")
{
    constexpr Ray2f ray = Ray2f::FromOriginAndDirection(Point2f(0.0f, 0.0f), UnitVector2f::Right());
    constexpr float distance = 5.0f;
    const LineSegment2f segment = ToLineSegment(ray, distance);
    REQUIRE(segment.StartPoint() == Point2f(0.0f, 0.0f));
    REQUIRE(segment.EndPoint() == Point2f(5.0f, 0.0f));
}

TEST_CASE("ShapeMath::ToLineSegment (Ray3 -> LineSegment3)", "[ShapeMath]")
{
    constexpr Ray3f ray(Point3f(0.0f, 0.0f, 0.0f), UnitVector3f::Forward());
    constexpr float distance = 5.0f;
    const LineSegment3f segment = ToLineSegment(ray, distance);
    REQUIRE(segment.StartPoint() == Point3f(0.0f, 0.0f, 0.0f));
    REQUIRE(segment.EndPoint() == Point3f(0.0f, 0.0f, 5.0f));
}

TEST_CASE("ShapeMath::ToPlane (Triangle3 -> Plane)", "ShapeMath")
{
    constexpr Triangle3f triangle(Point3f(0.0f, 0.0f, 1.0f), Point3f(1.0f, 0.0f, 0.0f), Point3f(0.0f, 1.0f, 0.0f));
    const Planef plane = ToPlane(triangle);
    REQUIRE(plane.GetPoint() == Point3f(0.0f, 0.0f, 1.0f));
    REQUIRE(NearlyEqual(plane.GetNormal(), UnitVector3f(1.0f, 1.0f, 1.0f)));
}

TEST_CASE("ShapeMath::Remap (AABB2)", "[ShapeMath]")
{
    constexpr AABB2f from = AABB2f::FromMinAndMax(Point2f(0.0f, 0.0f), Point2f(10.0f, 10.0f));
    constexpr AABB2f to = AABB2f::FromMinAndMax(Point2f(100.0f, 100.0f), Point2f(200.0f, 200.0f));
    constexpr Point2f pointInFrom(5.0f, 5.0f);
    const Point2f remappedPoint = Remap(pointInFrom, from, to);
    REQUIRE(remappedPoint == Point2f(150.0f, 150.0f));
}

TEST_CASE("ShapeMath::Remap (AABB3)", "[ShapeMath]")
{
    constexpr AABB3f from = AABB3f::FromMinAndMax(Point3f(0.0f, 0.0f, 0.0f), Point3f(10.0f, 10.0f, 10.0f));
    constexpr AABB3f to = AABB3f::FromMinAndMax(Point3f(100.0f, 100.0f, 100.0f), Point3f(200.0f, 200.0f, 200.0f));
    constexpr Point3f pointInFrom(5.0f, 5.0f, 5.0f);
    const Point3f remappedPoint = Remap(pointInFrom, from, to);
    REQUIRE(remappedPoint == Point3f(150.0f, 150.0f, 150.0f));
}

TEST_CASE("ShapeMath::Remap0To1 (AABB2)", "[ShapeMath]")
{
    constexpr AABB2f from = AABB2f::FromMinAndMax(Point2f(0.0f, 0.0f), Point2f(10.0f, 10.0f));
    constexpr Point2f pointInFrom(5.0f, 5.0f);
    const Point2f remappedPoint = Remap0To1(pointInFrom, from);
    REQUIRE(remappedPoint == Point2f(0.5f, 0.5f));
}

TEST_CASE("ShapeMath::Remap0To1 (AABB3)", "[ShapeMath]")
{
    constexpr AABB3f from = AABB3f::FromMinAndMax(Point3f(0.0f, 0.0f, 0.0f), Point3f(10.0f, 10.0f, 10.0f));
    constexpr Point3f pointInFrom(5.0f, 5.0f, 5.0f);
    const Point3f remappedPoint = Remap0To1(pointInFrom, from);
    REQUIRE(remappedPoint == Point3f(0.5f, 0.5f, 0.5f));
}

TEST_CASE("ShapeMath::OffsetAABB", "[ShapeMath]")
{
    constexpr AABB2f aabb = AABB2f::FromMinAndMax(Point2f(1.0f, 1.0f), Point2f(3.0f, 3.0f));
    constexpr Vector2f offset(2.0f, 2.0f);
    constexpr AABB2f offsetAABB = OffsetAABB(aabb, offset);
    REQUIRE(offsetAABB.GetMin() == Point2f(3.0f, 3.0f));
    REQUIRE(offsetAABB.GetMax() == Point2f(5.0f, 5.0f));
}

TEST_CASE("ShapeMath::ToAABB2XY", "[ShapeMath]")
{
    constexpr AABB3f aabb3 = AABB3f::FromMinAndMax(Point3f(1.0f, 2.0f, 3.0f), Point3f(4.0f, 5.0f, 6.0f));
    constexpr AABB2f aabb2xy = ToAABB2XY(aabb3);
    REQUIRE(aabb2xy.GetMin() == Point2f(1.0f, 2.0f));
    REQUIRE(aabb2xy.GetMax() == Point2f(4.0f, 5.0f));
}

TEST_CASE("ShapeMath::ToAABB2YZ", "[ShapeMath]")
{
    constexpr AABB3f aabb3 = AABB3f::FromMinAndMax(Point3f(1.0f, 2.0f, 3.0f), Point3f(4.0f, 5.0f, 6.0f));
    constexpr AABB2f aabb2yz = ToAABB2YZ(aabb3);
    REQUIRE(aabb2yz.GetMin() == Point2f(2.0f, 3.0f));
    REQUIRE(aabb2yz.GetMax() == Point2f(5.0f, 6.0f));
}

TEST_CASE("ShapeMath::ToAABB2XZ", "[ShapeMath]")
{
    constexpr AABB3f aabb3 = AABB3f::FromMinAndMax(Point3f(1.0f, 2.0f, 3.0f), Point3f(4.0f, 5.0f, 6.0f));
    constexpr AABB2f aabb2xz = ToAABB2XZ(aabb3);
    REQUIRE(aabb2xz.GetMin() == Point2f(1.0f, 3.0f));
    REQUIRE(aabb2xz.GetMax() == Point2f(4.0f, 6.0f));
}

TEST_CASE("ShapeMath::ToAABB2 (LineSegment2)", "[ShapeMath]")
{
    constexpr LineSegment2d segment = LineSegment2d::FromPoints(Point2d(1.0f, 2.0f), Point2d(4.0f, 5.0f));
    const AABB2d aabb2 = ToAABB2(segment);
    REQUIRE(aabb2.GetMin() == Point2d(1.0f, 2.0f));
    REQUIRE(aabb2.GetMax() == Point2d(4.0f, 5.0f));
}

TEST_CASE("ShapeMath::ToAABB3 (LineSegment3)", "[ShapeMath]")
{
    constexpr LineSegment3d segment = LineSegment3d::FromPoints(Point3d(1.0f, 2.0f, 3.0f), Point3d(4.0f, 5.0f, 6.0f));
    const AABB3d aabb3 = ToAABB3(segment);
    REQUIRE(aabb3.GetMin() == Point3d(1.0f, 2.0f, 3.0f));
    REQUIRE(aabb3.GetMax() == Point3d(4.0f, 5.0f, 6.0f));
}

TEST_CASE("ShapeMath::GetPlaneXPos", "[ShapeMath]")
{
    SECTION("Plane with non-zero X normal")
    {
        constexpr Planed plane = Planed::FromPointAndNormal(Point3d(0.0f, 0.0f, 0.0f), UnitVector3d::Right());
        constexpr auto xPos = GetPlaneXPos(plane, 5.0, 10.0);
        REQUIRE(xPos.has_value());
        REQUIRE(xPos.value() == 0.0f); // Since the plane is vertical at x=0
    }
    SECTION("Plane with zero X normal")
    {
        constexpr Planed plane = Planed::FromPointAndNormal(Point3d(0.0f, 0.0f, 0.0f), UnitVector3d::Up());
        constexpr auto xPos = GetPlaneXPos(plane, 5.0, 10.0);
        REQUIRE(!xPos.has_value());
    }
}

TEST_CASE("ShapeMath::GetPlaneYPos", "[ShapeMath]")
{
    SECTION("Plane with non-zero Y normal")
    {
        constexpr Planed plane = Planed::FromPointAndNormal(Point3d(0.0f, 0.0f, 0.0f), UnitVector3d::Up());
        constexpr auto yPos = GetPlaneYPos(plane, 5.0, 10.0);
        REQUIRE(yPos.has_value());
        REQUIRE(yPos.value() == 0.0f); // Since the plane is horizontal at y=0
    }
    SECTION("Plane with zero Y normal")
    {
        constexpr Planed plane = Planed::FromPointAndNormal(Point3d(0.0f, 0.0f, 0.0f), UnitVector3d::Right());
        constexpr auto yPos = GetPlaneYPos(plane, 5.0, 10.0);
        REQUIRE(!yPos.has_value());
    }
}

TEST_CASE("ShapeMath::GetPlaneZPos", "[ShapeMath]")
{
    SECTION("Plane with non-zero Z normal")
    {
        constexpr Planed plane = Planed::FromPointAndNormal(Point3d(0.0f, 0.0f, 0.0f), UnitVector3d::Forward());
        constexpr auto zPos = GetPlaneZPos(plane, 5.0, 10.0);
        REQUIRE(zPos.has_value());
        REQUIRE(zPos.value() == 0.0f); // Since the plane is vertical at z=0
    }
    SECTION("Plane with zero Z normal")
    {
        constexpr Planed plane = Planed::FromPointAndNormal(Point3d(0.0f, 0.0f, 0.0f), UnitVector3d::Up());
        constexpr auto zPos = GetPlaneZPos(plane, 5.0, 10.0);
        REQUIRE(!zPos.has_value());
    }
}

TEST_CASE("ShapeMath::IsOnNormalSide", "[ShapeMath]")
{
    constexpr Planed plane = Planed::FromPointAndNormal(Point3d(0.0f, 0.0f, 0.0f), UnitVector3d::Up());
    constexpr Point3d pointAbove(0.0f, 1.0f, 0.0f);
    constexpr Point3d pointBelow(0.0f, -1.0f, 0.0f);
    constexpr Point3d pointOnPlane(0.0f, 0.0f, 0.0f);
    REQUIRE(IsOnNormalSide(pointAbove, plane));
    REQUIRE(!IsOnNormalSide(pointBelow, plane));
    REQUIRE(!IsOnNormalSide(pointOnPlane, plane));
}

TEST_CASE("ShapeMath::GetNormal", "[ShapeMath]")
{
    constexpr Triangle3d triangle(Point3d(0.0f, 0.0f, 0.0f), Point3d(1.0f, 0.0f, 0.0f), Point3d(0.0f, 1.0f, 0.0f));
    const UnitVector3d normal = GetNormal(triangle);
    REQUIRE(normal == UnitVector3d::Forward());
}

TEST_CASE("ShapeMath::GetEdgeNormals", "[ShapeMath]")
{
    constexpr Triangle3d triangle(Point3d(0.0f, 0.0f, 0.0f), Point3d(0.0f, 1.0f, 0.0f), Point3d(1.0f, 1.0f, 0.0f));

    const auto a = GetEdgeNormals(triangle);

    REQUIRE(a[0] == UnitVector3d(-1.0f, 0.0f, 0.0f));
    REQUIRE(a[1] == UnitVector3d(0.0f, 1.0f, 0.0f));
    REQUIRE(NearlyEqual(a[2], UnitVector3d(1.f, -1.f, 0.f)));
}

TEST_CASE("ShapeMath::GetLength (LineSegment2)", "[ShapeMath]")
{
    constexpr LineSegment2d segment = LineSegment2d::FromPoints(Point2d(0.0f, 0.0f), Point2d(3.0f, 4.0f));
    const double length = GetLength(segment);
    REQUIRE(length == 5.0);
}

TEST_CASE("ShapeMath::GetLength (LineSegment3)", "[ShapeMath]")
{
    constexpr LineSegment3d segment = LineSegment3d::FromPoints(Point3d(0.0f, 0.0f, 0.0f), Point3d(1.0f, 2.0f, 2.0f));
    const double length = GetLength(segment);
    REQUIRE(length == 3.0);
}

TEST_CASE("ShapeMath::Lerp (LineSegment2)", "[ShapeMath]")
{
    constexpr LineSegment2d segment = LineSegment2d::FromPoints(Point2d(0.0f, 0.0f), Point2d(10.0f, 10.0f));
    const Point2d pointAtHalf = Lerp(segment, 0.5);
    REQUIRE(pointAtHalf == Point2d(5.0f, 5.0f));
}

TEST_CASE("ShapeMath::Lerp (LineSegment3)", "[ShapeMath]")
{
    constexpr LineSegment3d segment = LineSegment3d::FromPoints(Point3d(0.0f, 0.0f, 0.0f), Point3d(10.0f, 10.0f, 10.0f));
    const Point3d pointAtHalf = Lerp(segment, 0.5);
    REQUIRE(pointAtHalf == Point3d(5.0f, 5.0f, 5.0f));
}

TEST_CASE("ShapeMath::GetDirection", "[ShapeMath]")
{
    constexpr LineSegment2d segment = LineSegment2d::FromPoints(Point2d(0.0f, 0.0f), Point2d(10.0f, 10.0f));
    const UnitVector2d direction2d = GetDirection(segment);
    REQUIRE(direction2d == UnitVector2d(1, 1));
}

TEST_CASE("ShapeMath::GetDirection (LineSegment3)", "[ShapeMath]")
{
    constexpr LineSegment3d segment = LineSegment3d::FromPoints(Point3d(0.0f, 0.0f, 0.0f), Point3d(1.0f, 1.0f, 1.0f));
    const UnitVector3d direction3d = GetDirection(segment);
    REQUIRE(direction3d == UnitVector3d(1, 1, 1));
}

TEST_CASE("ShapeMath::GetNormal (LineSegment2)", "[ShapeMath]")
{
    constexpr LineSegment2d segment = LineSegment2d::FromPoints(Point2d(0.0f, 0.0f), Point2d(1.0f, 0.0f));
    const UnitVector2d normal = GetNormal(segment);
    REQUIRE(normal == UnitVector2d::Up());
}

TEST_CASE("ShapeMath::GetBounds (LineSegment2)", "[ShapeMath]")
{
    constexpr LineSegment2d segment = LineSegment2d::FromPoints(Point2d(-1.0f, 2.0f), Point2d(3.0f, -4.0f));
    const AABB2d bounds = GetBounds(segment);
    REQUIRE(bounds.GetMin() == Point2d(-1.0f, -4.0f));
    REQUIRE(bounds.GetMax() == Point2d(3.0f, 2.0f));
}

TEST_CASE("ShapeMath::GetBounds (LineSegment3)", "[ShapeMath]")
{
    constexpr LineSegment3d segment = LineSegment3d::FromPoints(Point3d(-1.0f, 2.0f, 0.0f), Point3d(3.0f, -4.0f, 5.0f));
    const AABB3d bounds = GetBounds(segment);
    REQUIRE(bounds.GetMin() == Point3d(-1.0f, -4.0f, 0.0f));
    REQUIRE(bounds.GetMax() == Point3d(3.0f, 2.0f, 5.0f));
}

TEST_CASE("ShapeMath::GetBounds (Triangle3)", "[ShapeMath]")
{
    constexpr Triangle3d triangle(Point3d(0.0f, 0.0f, 0.0f), Point3d(1.0f, 2.0f, 3.0f), Point3d(-1.0f, -2.0f, -3.0f));
    const AABB3d bounds = GetBounds(triangle);
    REQUIRE(bounds.GetMin() == Point3d(-1.0f, -2.0f, -3.0f));
    REQUIRE(bounds.GetMax() == Point3d(1.0f, 2.0f, 3.0f));
}

TEST_CASE("ShapeMath::GetBounds (Sphere)", "[ShapeMath]")
{
    const Sphered sphere = Sphered::FromCenterAndRadius(Point3d(5.0, 1.0, -4.0), Radiusd(4.0));
    const AABB3d bounds = GetBounds(sphere);
    REQUIRE(bounds.GetMin() == Point3d(1.0, -3.0, -8.0));
    REQUIRE(bounds.GetMax() == Point3d(9.0, 5.0, 0.0));
}

TEST_CASE("ShapeMath::GetBounds (Disk)", "[ShapeMath]")
{
    const Diskd disk = Diskd(Point3d(5.0, 5.0, 5.0), UnitVector3d::Forward(), Radiusd(3.0));
    const AABB3d bounds = GetBounds(disk);
    REQUIRE(bounds.GetMin() == Point3d(2.0, 2.0, 5.0));
    REQUIRE(bounds.GetMax() == Point3d(8.0, 8.0, 5.0));
}

TEST_CASE("ShapeMath::GetDistance (Plane, Point3)", "[ShapeMath]")
{
    const Planed plane = Planed::FromPointAndNormal(Point3d(0, 0, 0), UnitVector3d(1, 1, 1));
    constexpr Point3d pointAbove(0, 5, 0);
    constexpr Point3d pointBelow(0, -3, 0);
    constexpr Point3d pointOnPlane(0, 0, 0);
    const double distanceAbove = GetDistance(plane, pointAbove);
    const double distanceBelow = GetDistance(plane, pointBelow);
    const double distanceOnPlane = GetDistance(plane, pointOnPlane);
    REQUIRE(NearlyEqual(distanceAbove, 5.0 / std::sqrt(3.0)));
    REQUIRE(NearlyEqual(distanceBelow, 3.0 / std::sqrt(3.0)));
    REQUIRE(NearlyEqual(distanceOnPlane, 0.0));
}

TEST_CASE("ShapeMath::GetDistance (Line3, Point3)", "[ShapeMath]")
{
    constexpr Line3d line = Line3d::FromPointAndDirection(Point3d(0, 0, 0), UnitVector3d::Right());
    constexpr Point3d point(3, 4, 5);
    const double distance = GetDistance(line, point);
    REQUIRE(NearlyEqual(distance, std::sqrt(4.0 * 4.0 + 5.0 * 5.0)));
}

TEST_CASE("ShapeMath::GetDistance (Line2, Point2)", "[ShapeMath]")
{
    constexpr Line2d line = Line2d::FromPointAndDirection(Point2d(0, 0), UnitVector2d::Right());
    constexpr Point2d point(3, 4);
    const double distance = GetDistance(line, point);
    REQUIRE(distance == Approx(4.0));
}

TEST_CASE("ShapeMath::GetProjectedPoint (Plane)", "[ShapeMath]")
{
    SECTION("Positive Side")
    {
        const Planed plane = Planed::FromPointAndNormal(Point3d::Zero(), UnitVector3d(1, 1, 1));

        const Point3d p = GetProjectedPoint(plane, Point3d(10, 10, 10));
        REQUIRE(p.x == Approx(0.0));
        REQUIRE(p.y == Approx(0.0));
        REQUIRE(p.z == Approx(0.0));
    }

    SECTION("Negative Side")
    {
        const Planed plane = Planed::FromPointAndNormal(Point3d::Zero(), UnitVector3d(1, 1, 1));
        const Point3d p = GetProjectedPoint(plane, Point3d(-10, -10, -10));
        REQUIRE(p.x == Approx(0.0));
        REQUIRE(p.y == Approx(0.0));
        REQUIRE(p.z == Approx(0.0));
    }
}

TEST_CASE("ShapeMath::GetProjectedPoint (Line3)", "[ShapeMath]")
{
    constexpr Line3f line = Line3f::FromPointAndDirection(Point3f(0, 2, 2.5f), UnitVector3f::Right());

    constexpr Point3f p = GetProjectedPoint(line, Point3f(10, 10, 7.4f));
    REQUIRE(p == Point3f(10.f, 2.f, 2.5f));
}

TEST_CASE("ShapeMath::GetProjectedPoint (Line2)", "[ShapeMath]")
{
    constexpr Line2d line = Line2d::FromPointAndDirection(Point2d(0, 2), UnitVector2d::Right());

    constexpr Point2d p = GetProjectedPoint(line, Point2d(10, 10));
    REQUIRE(p == Point2d(10.f, 2.f));
}

TEST_CASE("ShapeMath::GetIntersectionLine", "[ShapeMath]")
{
    SECTION("Parallel - Same normal")
    {
        constexpr Planed plane1 = Planed::FromPointAndNormal(Point3d(4, 1, 5), UnitVector3d::Up());
        constexpr Planed plane2 = Planed::FromPointAndNormal(Point3d(2, -17, 5), UnitVector3d::Up());
        const std::optional<Line3d> intersection = GetIntersectionLine(plane1, plane2);
        REQUIRE(!intersection.has_value());
    }
    SECTION("Parallel - Opposite normal")
    {
        constexpr Planed plane1 = Planed::FromPointAndNormal(Point3d(4, 1, 5), UnitVector3d::Left());
        constexpr Planed plane2 = Planed::FromPointAndNormal(Point3d(2, -17, 5), UnitVector3d::Right());
        const std::optional<Line3d> intersection = GetIntersectionLine(plane1, plane2);
        REQUIRE(!intersection.has_value());
    }
    SECTION("Intersection")
    {
        constexpr Planed plane1 = Planed::FromPointAndNormal(Point3d(1, 1, 1), UnitVector3d::Right());
        constexpr Planed plane2 = Planed::FromPointAndNormal(Point3d(4, 1, 5), UnitVector3d::Up());
        const std::optional<Line3d> intersection = GetIntersectionLine(plane1, plane2);
        REQUIRE(intersection.has_value());
        REQUIRE((intersection->GetDirection() == UnitVector3d::Forward() || intersection->GetDirection() == UnitVector3d::Backward()));
        REQUIRE(intersection->GetPoint().x == 1);
        REQUIRE(intersection->GetPoint().y == 1);
    }
    SECTION("Intersection - flipped normal")
    {
        constexpr Planed plane1 = Planed::FromPointAndNormal(Point3d(1, 1, 1), UnitVector3d::Left());
        constexpr Planed plane2 = Planed::FromPointAndNormal(Point3d(4, 1, 5), UnitVector3d::Up());
        const std::optional<Line3d> intersection = GetIntersectionLine(plane1, plane2);
        REQUIRE(intersection.has_value());
        REQUIRE((intersection->GetDirection() == UnitVector3d::Forward() || intersection->GetDirection() == UnitVector3d::Backward()));
        REQUIRE(intersection->GetPoint().x == 1);
        REQUIRE(intersection->GetPoint().y == 1);
    }
}

TEST_CASE("ShapeMath::GetArea (AABB2)", "[ShapeMath]")
{
    constexpr AABB2d aabb = AABB2d::FromMinAndMax(Point2d(0, 0), Point2d(5, 10));
    constexpr double area = GetArea(aabb);
    REQUIRE(area == 50.0);
}

TEST_CASE("ShapeMath::GetArea (Circle)", "[ShapeMath]")
{
    constexpr Circled circle = Circled::FromCenterAndRadius(Point2d(0, 0), Radiusd(5.0));
    constexpr double area = GetArea(circle);
    REQUIRE(area == Approx(78.53981633974483));
}

TEST_CASE("ShapeMath::GetVolume (AABB3)", "[ShapeMath]")
{
    constexpr AABB3d aabb = AABB3d::FromMinAndMax(Point3d(0, 0, 0), Point3d(5, 5, 10));
    constexpr double volume = GetVolume(aabb);
    REQUIRE(volume == 250.0);
}

TEST_CASE("ShapeMath::Measure (AABB2)", "[ShapeMath]")
{
    constexpr AABB2d aabb = AABB2d::FromMinAndMax(Point2d(0, 0), Point2d(5, 10));
    constexpr double measure = Measure(aabb);
    REQUIRE(measure == 50.0); // Perimeter
}

TEST_CASE("ShapeMath::Measure (AABB3)", "[ShapeMath]")
{
    constexpr AABB3d aabb = AABB3d::FromMinAndMax(Point3d(0, 0, 0), Point3d(5, 5, 10));
    constexpr double measure = Measure(aabb);
    REQUIRE(measure == 250.0); // Surface Area
}

TEST_CASE("ShapeMath::GetOverlap (AABB2)", "[ShapeMath]")
{
    constexpr AABB2f a = AABB2f::FromMinAndMax(Point2f(0, 0), Point2f(5, 5));
    constexpr AABB2f b = AABB2f::FromMinAndMax(Point2f(3, 3), Point2f(7, 7));
    constexpr std::optional<AABB2f> overlap = GetOverlap(a, b);
    REQUIRE(overlap.has_value());
    REQUIRE(overlap->GetMin() == Point2f(3.f, 3.f));
    REQUIRE(overlap->GetMax() == Point2f(5.f, 5.f));
}

TEST_CASE("Get Overlap AABB3")
{
    constexpr AABB3f a = AABB3f::FromMinAndMax(Point3f(0, 0, 0), Point3f(5, 5, 5));
    constexpr AABB3f b = AABB3f::FromMinAndMax(Point3f(3, 3, 3), Point3f(7, 7, 7));
    constexpr std::optional<AABB3f> overlap = GetOverlap(a, b);
    REQUIRE(overlap.has_value());
    REQUIRE(overlap->GetMin() == Point3f(3.f, 3.f, 3.f));
    REQUIRE(overlap->GetMax() == Point3f(5.f, 5.f, 5.f));
}

TEST_CASE("Get Area AABB2")
{
    constexpr AABB2f aabb = AABB2f::FromMinAndMax(Point2f(0, 0), Point2f(5, 5));
    constexpr float area = GetArea(aabb);
    REQUIRE(area == 25.0f);
}

TEST_CASE("Get Volume AABB3")
{
    constexpr AABB3f aabb = AABB3f::FromMinAndMax(Point3f(0, 0, 0), Point3f(5, 5, 5));
    constexpr float volume = GetVolume(aabb);
    REQUIRE(volume == 125.0f);
}

TEST_CASE("Get Overlap Percentage AABB2")
{
    constexpr AABB2f a = AABB2f::FromMinAndMax(Point2f(0, 0), Point2f(5, 5));
    constexpr AABB2f b = AABB2f::FromMinAndMax(Point2f(2.5f, 2.5f), Point2f(7, 7));
    constexpr Percentf overlapPercentage = GetOverlapPercentage(a, b);
    REQUIRE(overlapPercentage == Percentf(0.25f));
}

TEST_CASE("Get Overlap Percentage AABB3")
{
    constexpr AABB3f a = AABB3f::FromMinAndMax(Point3f(0, 0, 0), Point3f(5, 5, 5));
    constexpr AABB3f b = AABB3f::FromMinAndMax(Point3f(2.5f, 2.5f, 2.5f), Point3f(7, 7, 7));
    constexpr Percentf overlapPercentage = GetOverlapPercentage(a, b);
    REQUIRE(overlapPercentage == Percentf(0.125f));
}

TEST_CASE("Get Overlap Percentage AABB2 No Overlap")
{
    constexpr AABB2f a = AABB2f::FromMinAndMax(Point2f(0, 0), Point2f(5, 5));
    constexpr AABB2f b = AABB2f::FromMinAndMax(Point2f(6, 6), Point2f(10, 10));
    constexpr Percentf overlapPercentage = GetOverlapPercentage(a, b);

    REQUIRE(overlapPercentage == Percentf(0.0f));
}

TEST_CASE("Get Overlap Percentage AABB3 No Overlap")
{
    constexpr AABB3f a = AABB3f::FromMinAndMax(Point3f(0, 0, 0), Point3f(5, 5, 5));
    constexpr AABB3f b = AABB3f::FromMinAndMax(Point3f(6, 6, 6), Point3f(10, 10, 10));
    constexpr Percentf overlapPercentage = GetOverlapPercentage(a, b);
    REQUIRE(overlapPercentage == Percentf(0.0f));
}

TEST_CASE("Get Overlap Percentage AABB2 Full Overlap")
{
    constexpr AABB2f a = AABB2f::FromMinAndMax(Point2f(0, 0), Point2f(5, 5));
    constexpr AABB2f b = AABB2f::FromMinAndMax(Point2f(1, 1), Point2f(4, 4));
    constexpr Percentf overlapPercentage = GetOverlapPercentage(a, b);
    REQUIRE(overlapPercentage == Percentf(0.36f));
}

TEST_CASE("Get Overlap Percentage AABB3 Full Overlap")
{
    constexpr AABB3f a = AABB3f::FromMinAndMax(Point3f(0, 0, 0), Point3f(5, 5, 5));
    constexpr AABB3f b = AABB3f::FromMinAndMax(Point3f(1, 1, 1), Point3f(4, 4, 4));
    constexpr Percentf overlapPercentage = GetOverlapPercentage(a, b);
    REQUIRE(overlapPercentage == Percentf(0.216f));
}

TEST_CASE("Get Area Triangle2")
{
    {
        constexpr Triangle2f triangle(Point2f(0.0f, 0.0f), Point2f(0.0f, 1.0f), Point2f(1.0f, 1.0f));
        constexpr float area = GetArea(triangle);
        REQUIRE(area == 0.5f);
    }

    {
        constexpr Triangle2f triangle(Point2f(0.0f, 0.0f), Point2f(-1.0f, 1.0f), Point2f(1.0f, 1.0f));
        constexpr float area = GetArea(triangle);
        REQUIRE(area == 1.f);
    }

    {
        constexpr AABB3i aabb = AABB3i::FromMinAndMax(Point3i(0, 0, 0), Point3i(5, 5, 5));

        const auto corners = GetCorners<int, std::vector<Point3i>>(aabb);
    }
}

TEST_CASE("Get Surface Area Sphere")
{
    constexpr Spheref sphere = Spheref::FromCenterAndRadius(Point3f::Zero(), Radiusf(1.0f));
    constexpr float area = GetSurfaceArea(sphere);
    REQUIRE(Abs(area - 4.0f * PI<float>) < 0.0001f);
}

TEST_CASE("Get Volume Sphere")
{
    constexpr Spheref sphere = Spheref::FromCenterAndRadius(Point3f::Zero(), Radiusf(1.0f));
    constexpr float volume = GetVolume(sphere);
    REQUIRE(Abs(volume - (4.0f / 3.0f) * PI<float>) < 0.0001f);
}

TEST_CASE("Get Sphere Disk Facing Point")
{
    constexpr Spheref sphere = Spheref::FromCenterAndRadius(Point3f::Zero(), Radiusf(1.0f));
    constexpr Point3f point(0.0f, 0.0f, -5.0f);
    const auto disk = GetSphereDiskFacingPoint(sphere, point);

    REQUIRE(disk.has_value());

    REQUIRE(disk->GetCenter() == Point3f(0.0f, 0.0f, -1.0f));
    REQUIRE(disk->GetNormal() == UnitVector3f::Backward());
    REQUIRE(disk->GetRadius() == Radiusf(1.0f));
}

TEST_CASE("Get Sphere Disk Facing Point Error")
{
    constexpr Spheref sphere = Spheref::FromCenterAndRadius(Point3f::Zero(), Radiusf(1.0f));
    constexpr Point3f point = sphere.GetCenter();
    const auto disk = GetSphereDiskFacingPoint(sphere, point);
    REQUIRE(!disk.has_value());
    REQUIRE(disk.error() == eSphereDiskFacingPointError::PointEqualsCenter);
}

TEST_CASE("Get Visible Sphere Disk Facing Point")
{
    constexpr Spheref sphere = Spheref::FromCenterAndRadius(Point3f::Zero(), Radiusf(1.0f));
    constexpr Point3f viewPosition(0.0f, 0.0f, -5.0f);
    const auto disk = GetVisibleSphereDiskFacingPoint(sphere, viewPosition);
    REQUIRE(disk.has_value());
}

TEST_CASE("Get Percentage Visible")
{
    /*constexpr Spheref sphere(Point3f::Zero(), 1.0f);

    constexpr Point3f viewPosition(0.0f, 0.0f, -5.0f);

    constexpr auto percentageVisible = GetPercentageAreaVisible(sphere, viewPosition);*/
}