#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/Math/VectorMath.hpp"

using namespace Simple;

TEST_CASE("VectorMath::GetX/Y/Z (Point3)", "[VectorMath]")
{
    const Point3f point(1.0f, 2.0f, 3.0f);
    REQUIRE(GetX(point) == 1.0f);
    REQUIRE(GetY(point) == 2.0f);
    REQUIRE(GetZ(point) == 3.0f);
    Point3f mutablePoint(4.0f, 5.0f, 6.0f);
    REQUIRE(GetX(mutablePoint) == 4.0f);
    REQUIRE(GetY(mutablePoint) == 5.0f);
    REQUIRE(GetZ(mutablePoint) == 6.0f);
    GetX(mutablePoint) = 7.0f;
    GetY(mutablePoint) = 8.0f;
    GetZ(mutablePoint) = 9.0f;
    REQUIRE(mutablePoint.x == 7.0f);
    REQUIRE(mutablePoint.y == 8.0f);
    REQUIRE(mutablePoint.z == 9.0f);
}

TEST_CASE("VectorMath::GetX/Y/Z (Vector3)", "[VectorMath]")
{
    const Vector3f vector(1.0f, 2.0f, 3.0f);
    REQUIRE(GetX(vector) == 1.0f);
    REQUIRE(GetY(vector) == 2.0f);
    REQUIRE(GetZ(vector) == 3.0f);
    Vector3f mutableVector(4.0f, 5.0f, 6.0f);
    REQUIRE(GetX(mutableVector) == 4.0f);
    REQUIRE(GetY(mutableVector) == 5.0f);
    REQUIRE(GetZ(mutableVector) == 6.0f);
    GetX(mutableVector) = 7.0f;
    GetY(mutableVector) = 8.0f;
    GetZ(mutableVector) = 9.0f;
    REQUIRE(mutableVector.x == 7.0f);
    REQUIRE(mutableVector.y == 8.0f);
    REQUIRE(mutableVector.z == 9.0f);
}

TEST_CASE("VectorMath::GetX/Y/Z (UnitVector3)", "[VectorMath]")
{
    const UnitVector3f vector(1.0f, 0.0f, 0.0f);
    REQUIRE(GetX(vector) == 1.0f);
    REQUIRE(GetY(vector) == 0.0f);
    REQUIRE(GetZ(vector) == 0.0f);
}

TEST_CASE("VectorMath::ToVector4 (Vector3)", "[VectorMath]")
{
    constexpr Vector3f vector(1.0f, 2.0f, 3.0f);
    constexpr Vector4f vec4DefaultW = ToVector4(vector);
    REQUIRE(vec4DefaultW.x == 1.0f);
    REQUIRE(vec4DefaultW.y == 2.0f);
    REQUIRE(vec4DefaultW.z == 3.0f);
    REQUIRE(vec4DefaultW.w == 1.0f);
    constexpr Vector4f vec4CustomW = ToVector4(vector, 4.0f);
    REQUIRE(vec4CustomW.x == 1.0f);
    REQUIRE(vec4CustomW.y == 2.0f);
    REQUIRE(vec4CustomW.z == 3.0f);
    REQUIRE(vec4CustomW.w == 4.0f);
}

TEST_CASE("VectorMath::ToVector4 (Point3)", "[VectorMath]")
{
    constexpr Point3f point(1.0f, 2.0f, 3.0f);
    constexpr Vector4f vec4DefaultW = ToVector4(point);
    REQUIRE(vec4DefaultW.x == 1.0f);
    REQUIRE(vec4DefaultW.y == 2.0f);
    REQUIRE(vec4DefaultW.z == 3.0f);
    REQUIRE(vec4DefaultW.w == 1.0f);
    constexpr Vector4f vec4CustomW = ToVector4(point, 4.0f);
    REQUIRE(vec4CustomW.x == 1.0f);
    REQUIRE(vec4CustomW.y == 2.0f);
    REQUIRE(vec4CustomW.z == 3.0f);
    REQUIRE(vec4CustomW.w == 4.0f);
}

TEST_CASE("VectorMath::ToVector3 (Vector4)", "[VectorMath]")
{
    constexpr Vector4f v4(1.0f, 0.0f, 0.0f, 1.f);
    constexpr Vector3f vector = ToVector3(v4);
    REQUIRE(vector.x == 1.0f);
    REQUIRE(vector.y == 0.0f);
    REQUIRE(vector.z == 0.0f);
}

TEST_CASE("VectorMath::ToVector3 (UnitVector3)", "[VectorMath]")
{
    constexpr UnitVector3f unitVector(1.0f, 0.0f, 0.0f);
    constexpr Vector3f vector = ToVector3(unitVector);
    REQUIRE(vector.x == 1.0f);
    REQUIRE(vector.y == 0.0f);
    REQUIRE(vector.z == 0.0f);
}

TEST_CASE("VectorMath::ToVector3XY", "[VectorMath]")
{
    constexpr Vector2f vec2(1.0f, 2.0f);
    constexpr Vector3f vectorDefaultZ = ToVector3XY(vec2);
    REQUIRE(vectorDefaultZ.x == 1.0f);
    REQUIRE(vectorDefaultZ.y == 2.0f);
    REQUIRE(vectorDefaultZ.z == 0.0f);
    constexpr Vector3f vectorCustomZ = ToVector3XY(vec2, 3.0f);
    REQUIRE(vectorCustomZ.x == 1.0f);
    REQUIRE(vectorCustomZ.y == 2.0f);
    REQUIRE(vectorCustomZ.z == 3.0f);
}

TEST_CASE("VectorMath::ToVector3XZ", "[VectorMath]")
{
    constexpr Vector2f vec2(1.0f, 2.0f);
    constexpr Vector3f vectorDefaultY = ToVector3XZ(vec2);
    REQUIRE(vectorDefaultY.x == 1.0f);
    REQUIRE(vectorDefaultY.y == 0.0f);
    REQUIRE(vectorDefaultY.z == 2.0f);
    constexpr Vector3f vectorCustomY = ToVector3XZ(vec2, 3.0f);
    REQUIRE(vectorCustomY.x == 1.0f);
    REQUIRE(vectorCustomY.y == 3.0f);
    REQUIRE(vectorCustomY.z == 2.0f);
}

TEST_CASE("VectorMath::ToVector3YZ", "[VectorMath]")
{
    constexpr Vector2f vec2(1.0f, 2.0f);
    constexpr Vector3f vectorDefaultX = ToVector3YZ(vec2);
    REQUIRE(vectorDefaultX.x == 0.0f);
    REQUIRE(vectorDefaultX.y == 1.0f);
    REQUIRE(vectorDefaultX.z == 2.0f);
    constexpr Vector3f vectorCustomX = ToVector3YZ(vec2, 3.0f);
    REQUIRE(vectorCustomX.x == 3.0f);
    REQUIRE(vectorCustomX.y == 1.0f);
    REQUIRE(vectorCustomX.z == 2.0f);
}

TEST_CASE("VectorMath::ToVector2XY", "[VectorMath]")
{
    constexpr Vector3f vec3(1.0f, 2.0f, 3.0f);
    constexpr Vector2f vector2 = ToVector2XY(vec3);
    REQUIRE(vector2.x == 1.0f);
    REQUIRE(vector2.y == 2.0f);
}

TEST_CASE("VectorMath::ToVector2XZ", "[VectorMath]")
{
    constexpr Vector3f vec3(1.0f, 2.0f, 3.0f);
    constexpr Vector2f vector2 = ToVector2XZ(vec3);
    REQUIRE(vector2.x == 1.0f);
    REQUIRE(vector2.y == 3.0f);
}

TEST_CASE("VectorMath::ToVector2YZ", "[VectorMath]")
{
    constexpr Vector3f vec3(1.0f, 2.0f, 3.0f);
    constexpr Vector2f vector2 = ToVector2YZ(vec3);
    REQUIRE(vector2.x == 2.0f);
    REQUIRE(vector2.y == 3.0f);
}

TEST_CASE("VectorMath::ToPoint3 (Vector4)", "[VectorMath]")
{
    constexpr Vector4f v4(1.0f, 2.0f, 3.0f, 1.f);
    constexpr Point3f point = ToPoint3(v4);
    REQUIRE(point.x == 1.0f);
    REQUIRE(point.y == 2.0f);
    REQUIRE(point.z == 3.0f);
}

TEST_CASE("VectorMath::ToPoint2XY", "[VectorMath]")
{
    constexpr Point3f p(1.0f, 2.0f, 3.f);
    constexpr Point2f pointDefaultZ = ToPoint2XY(p);
    REQUIRE(pointDefaultZ.x == 1.0f);
    REQUIRE(pointDefaultZ.y == 2.0f);
}

TEST_CASE("VectorMath::ToPoint2XZ", "[VectorMath]")
{
    constexpr Point3f p(1.0f, 2.0f, 3.f);
    constexpr Point2f pointDefaultY = ToPoint2XZ(p);
    REQUIRE(pointDefaultY.x == 1.0f);
    REQUIRE(pointDefaultY.y == 3.0f);
}

TEST_CASE("VectorMath::ToPoint2YZ", "[VectorMath]")
{
    constexpr Point3f p(1.0f, 2.0f, 3.f);
    constexpr Point2f pointDefaultX = ToPoint2YZ(p);
    REQUIRE(pointDefaultX.x == 2.0f);
    REQUIRE(pointDefaultX.y == 3.0f);
}

TEST_CASE("VectorMath::ToPoint3XY", "[VectorMath]")
{
    constexpr Point2f p(1.0f, 2.0f);
    constexpr Point3f pointDefaultZ = ToPoint3XY(p);
    REQUIRE(pointDefaultZ.x == 1.0f);
    REQUIRE(pointDefaultZ.y == 2.0f);
    REQUIRE(pointDefaultZ.z == 0.0f);
    constexpr Point3f pointCustomZ = ToPoint3XY(p, 3.0f);
    REQUIRE(pointCustomZ.x == 1.0f);
    REQUIRE(pointCustomZ.y == 2.0f);
    REQUIRE(pointCustomZ.z == 3.0f);
}

TEST_CASE("VectorMath::ToPoint3XZ", "[VectorMath]")
{
    constexpr Point2f p(1.0f, 2.0f);
    constexpr Point3f pointDefaultY = ToPoint3XZ(p);
    REQUIRE(pointDefaultY.x == 1.0f);
    REQUIRE(pointDefaultY.y == 0.0f);
    REQUIRE(pointDefaultY.z == 2.0f);
    constexpr Point3f pointCustomY = ToPoint3XZ(p, 3.0f);
    REQUIRE(pointCustomY.x == 1.0f);
    REQUIRE(pointCustomY.y == 3.0f);
    REQUIRE(pointCustomY.z == 2.0f);
}

TEST_CASE("VectorMath::ToPoint3YZ", "[VectorMath]")
{
    constexpr Point2f p(1.0f, 2.0f);
    constexpr Point3f pointDefaultX = ToPoint3YZ(p);
    REQUIRE(pointDefaultX.x == 0.0f);
    REQUIRE(pointDefaultX.y == 1.0f);
    REQUIRE(pointDefaultX.z == 2.0f);
    constexpr Point3f pointCustomX = ToPoint3YZ(p, 3.0f);
    REQUIRE(pointCustomX.x == 3.0f);
    REQUIRE(pointCustomX.y == 1.0f);
    REQUIRE(pointCustomX.z == 2.0f);
}

TEST_CASE("VectorMath::RemoveX (Vector3)", "[VectorMath]")
{
    constexpr Vector3f vec3(1.0f, 2.0f, 3.0f);
    constexpr Vector3f vectorNoX = RemoveX(vec3);
    REQUIRE(vectorNoX.x == 0.0f);
    REQUIRE(vectorNoX.y == 2.0f);
    REQUIRE(vectorNoX.z == 3.0f);
}

TEST_CASE("VectorMath::RemoveY (Vector3)", "[VectorMath]")
{
    constexpr Vector3f vec3(1.0f, 2.0f, 3.0f);
    constexpr Vector3f vectorNoY = RemoveY(vec3);
    REQUIRE(vectorNoY.x == 1.0f);
    REQUIRE(vectorNoY.y == 0.0f);
    REQUIRE(vectorNoY.z == 3.0f);
}

TEST_CASE("VectorMath::RemoveZ (Vector3)", "[VectorMath]")
{
    constexpr Vector3f vec3(1.0f, 2.0f, 3.0f);
    constexpr Vector3f vectorNoZ = RemoveZ(vec3);
    REQUIRE(vectorNoZ.x == 1.0f);
    REQUIRE(vectorNoZ.y == 2.0f);
    REQUIRE(vectorNoZ.z == 0.0f);
}

TEST_CASE("VectorMath::RemoveX (Point3)", "[VectorMath]")
{
    constexpr Point3f p3(1.0f, 2.0f, 3.0f);
    constexpr Point3f pointNoX = RemoveX(p3);
    REQUIRE(pointNoX.x == 0.0f);
    REQUIRE(pointNoX.y == 2.0f);
    REQUIRE(pointNoX.z == 3.0f);
}

TEST_CASE("VectorMath::RemoveY (Point3)", "[VectorMath]")
{
    constexpr Point3f p3(1.0f, 2.0f, 3.0f);
    constexpr Point3f pointNoY = RemoveY(p3);
    REQUIRE(pointNoY.x == 1.0f);
    REQUIRE(pointNoY.y == 0.0f);
    REQUIRE(pointNoY.z == 3.0f);
}

TEST_CASE("VectorMath::RemoveZ (Point3)", "[VectorMath]")
{
    constexpr Point3f p3(1.0f, 2.0f, 3.0f);
    constexpr Point3f pointNoZ = RemoveZ(p3);
    REQUIRE(pointNoZ.x == 1.0f);
    REQUIRE(pointNoZ.y == 2.0f);
    REQUIRE(pointNoZ.z == 0.0f);
}

TEST_CASE("VectorMath::RemoveX (UnitVector3)", "[VectorMath]")
{
    const UnitVector3f uv3(1.0f, 1.0f, 0.0f);
    const UnitVector3f vectorNoX = RemoveX(uv3);
    REQUIRE(vectorNoX.X() == 0.0f);
    REQUIRE(vectorNoX.Y() == 1.0f);
    REQUIRE(vectorNoX.Z() == 0.0f);
}

TEST_CASE("VectorMath::RemoveY (UnitVector3)", "[VectorMath]")
{
    const UnitVector3f uv3(1.0f, 1.0f, 0.0f);
    const UnitVector3f vectorNoY = RemoveY(uv3);
    REQUIRE(vectorNoY.X() == 1.0f);
    REQUIRE(vectorNoY.Y() == 0.0f);
    REQUIRE(vectorNoY.Z() == 0.0f);
}

TEST_CASE("VectorMath::RemoveZ (UnitVector3)", "[VectorMath]")
{
    const UnitVector3f uv3(1.0f, 0.0f, 1.0f);
    const UnitVector3f vectorNoZ = RemoveZ(uv3);
    REQUIRE(vectorNoZ.X() == 1.0f);
    REQUIRE(vectorNoZ.Y() == 0.0f);
    REQUIRE(vectorNoZ.Z() == 0.0f);
}

TEST_CASE("VectorMath::Dot (Vector3)", "[VectorMath]")
{
    const Vector3f a(1.0f, 2.0f, 3.0f);
    const Vector3f b(4.0f, -5.0f, 6.0f);
    const float result = Dot(a, b);
    REQUIRE(result == (1.0f * 4.0f + 2.0f * -5.0f + 3.0f * 6.0f));
}

TEST_CASE("VectorMath::Min (Vector2)", "[VectorMath]")
{
    constexpr Vector2f a(1.0f, 4.0f);
    constexpr Vector2f b(3.0f, 2.0f);
    constexpr Vector2f result = Min(a, b);
    REQUIRE(result.x == 1.0f);
    REQUIRE(result.y == 2.0f);
}

TEST_CASE("VectorMath::Max (Vector2)", "[VectorMath]")
{
    constexpr Vector2f a(1.0f, 4.0f);
    constexpr Vector2f b(3.0f, 2.0f);
    constexpr Vector2f result = Max(a, b);
    REQUIRE(result.x == 3.0f);
    REQUIRE(result.y == 4.0f);
}

TEST_CASE("VectorMath::Min (Vector3)", "[VectorMath]")
{
    constexpr Vector3f a(1.0f, 4.0f, 0.0f);
    constexpr Vector3f b(3.0f, 2.0f, 5.0f);
    constexpr Vector3f result = Min(a, b);
    REQUIRE(result.x == 1.0f);
    REQUIRE(result.y == 2.0f);
    REQUIRE(result.z == 0.0f);
}

TEST_CASE("VectorMath::Max (Vector3)", "[VectorMath]")
{
    constexpr Vector3f a(1.0f, 4.0f, 0.0f);
    constexpr Vector3f b(3.0f, 2.0f, 5.0f);
    constexpr Vector3f result = Max(a, b);
    REQUIRE(result.x == 3.0f);
    REQUIRE(result.y == 4.0f);
    REQUIRE(result.z == 5.0f);
}

TEST_CASE("VectorMath::Min (Point2)", "[VectorMath]")
{
    constexpr Point2f a(1.0f, 4.0f);
    constexpr Point2f b(3.0f, 2.0f);
    constexpr Point2f result = Min(a, b);
    REQUIRE(result.x == 1.0f);
    REQUIRE(result.y == 2.0f);
}

TEST_CASE("VectorMath::Max (Point2)", "[VectorMath]")
{
    constexpr Point2f a(1.0f, 4.0f);
    constexpr Point2f b(3.0f, 2.0f);
    constexpr Point2f result = Max(a, b);
    REQUIRE(result.x == 3.0f);
    REQUIRE(result.y == 4.0f);
}

TEST_CASE("VectorMath::Min (Point3)", "[VectorMath]")
{
    constexpr Point3f a(1.0f, 4.0f, 0.0f);
    constexpr Point3f b(3.0f, 2.0f, 5.0f);
    constexpr Point3f result = Min(a, b);
    REQUIRE(result.x == 1.0f);
    REQUIRE(result.y == 2.0f);
    REQUIRE(result.z == 0.0f);
}

TEST_CASE("VectorMath::Max (Point3)", "[VectorMath]")
{
    constexpr Point3f a(1.0f, 4.0f, 0.0f);
    constexpr Point3f b(3.0f, 2.0f, 5.0f);
    constexpr Point3f result = Max(a, b);
    REQUIRE(result.x == 3.0f);
    REQUIRE(result.y == 4.0f);
    REQUIRE(result.z == 5.0f);
}

TEST_CASE("VectorMath::Lerp (Point2f (float))", "[VectorMath]")
{
    constexpr Point2f a(5.0f, 5.0f);
    constexpr Point2f b(10.0f, 10.0f);

    constexpr Point2f result = Lerp(a, b, 0.5f);
    REQUIRE(result.x == 7.5f);
    REQUIRE(result.y == 7.5f);
}

TEST_CASE("VectorMath::Lerp (Point2i (float))", "[VectorMath]")
{
    constexpr Point2i a(5, 5);
    constexpr Point2i b(10, 10);
    constexpr Point2i result = Lerp(a, b, 0.5f);
    REQUIRE(result.x == 7);
    REQUIRE(result.y == 7);
}

TEST_CASE("VectorMath::Lerp (Point2i (int))", "[VectorMath]")
{
    constexpr Point2i a(5, 5);
    constexpr Point2i b(10, 10);
    constexpr Point2i result = Lerp(a, b, 2);
    REQUIRE(result.x == 15);
    REQUIRE(result.y == 15);
}

TEST_CASE("VectorMath::Lerp (Point2f (Vector2f))", "[VectorMath]")
{
    constexpr Point2f a(5.0f, 5.0f);
    constexpr Point2f b(10.0f, 10.0f);
    constexpr Point2f result = Lerp(a, b, Vector2f(0.5f, 0.2f));
    REQUIRE(result.x == 7.5f);
    REQUIRE(result.y == 6.f);
}

TEST_CASE("VectorMath::Lerp (Point2i (Vector2f))", "[VectorMath]")
{
    constexpr Point2i a(5, 5);
    constexpr Point2i b(10, 10);
    constexpr Point2i result = Lerp(a, b, Vector2f(0.5f, 0.2f));
    REQUIRE(result.x == 7);
    REQUIRE(result.y == 6);
}

TEST_CASE("VectorMath::Cross (Vector3)", "[VectorMath]")
{
    constexpr UnitVector3f right = UnitVector3f::Right();
    constexpr UnitVector3f up = UnitVector3f::Up();
    constexpr UnitVector3f forward = UnitVector3f::Forward();
    constexpr UnitVector3f left = UnitVector3f::Left();
    constexpr UnitVector3f down = UnitVector3f::Down();
    constexpr UnitVector3f backward = UnitVector3f::Backward();

    {
        const UnitVector3f result = Cross(right, up);
        REQUIRE(result == UnitVector3f::Forward());
    }
    {
        const UnitVector3f result = Cross(up, right);
        REQUIRE(result == UnitVector3f::Backward());
    }
    {
        const UnitVector3f result = Cross(up, forward);
        REQUIRE(result == UnitVector3f::Right());
    }
    {
        const UnitVector3f result = Cross(forward, up);
        REQUIRE(result == UnitVector3f::Left());
    }
    {
        const UnitVector3f result = Cross(forward, right);
        REQUIRE(result == UnitVector3f::Up());
    }
    {
        const UnitVector3f result = Cross(right, forward);
        REQUIRE(result == UnitVector3f::Down());
    }
    {
        const UnitVector3f result = Cross(backward, right);
        REQUIRE(result == UnitVector3f::Down());
    }
}

TEST_CASE("VectorMath::GetUnitVector")
{
    constexpr Point3f p1(10, 20.f, 100.f);
    constexpr Point3f p2(10.f, 21.f, 101.f);

    REQUIRE(GetUnitVector(p1, p2) == UnitVector3f(Vector3f(0, 1, 1)));
}


TEST_CASE("VectorMath::GetReflectionVector")
{
    {
        constexpr UnitVector3f forward = UnitVector3f::Forward();
        constexpr UnitVector3f backward = UnitVector3f::Backward();
        constexpr UnitVector3f reflection = GetReflectionVector(forward, backward);
        REQUIRE(reflection == backward);
    }
    {
        constexpr Vector3f direction(1, 1, 1);
        constexpr UnitVector3f normal = UnitVector3f::Backward();
        const UnitVector3f reflection = GetReflectionVector(direction, normal);
        REQUIRE(NearlyEqual(reflection, UnitVector3f(1, 1, -1)));
    }
}

TEST_CASE("VectorMath::GetPerpendicularVector")
{
    constexpr UnitVector3f forward = UnitVector3f::Forward();

    const UnitVector3f perp = GetPerpendicularVector(forward);

    REQUIRE(Dot(forward, perp) == 0.f);
}

TEST_CASE("VectorMath::InterpPoint")
{
    constexpr Point3f point1(10, 0, 0);
    constexpr Point3f target;

    const Point3f p = InterpPoint(point1, target, 10.f, 0.1f);

    REQUIRE(p == Point3f(9, 0, 0));
}

TEST_CASE("VectorMath::CalculateAngle")
{
    {
        const UnitVector3f forward = UnitVector3f::Forward();
        const UnitVector3f right = UnitVector3f::Right();

        const Radiansf angle = CalculateAngle(forward, right);
        REQUIRE(angle == ToRadians(Degreesf(90)));
    }

    {
        const UnitVector3f vec1(0, 1, 0);
        const UnitVector3f vec2(1, 0, 1);

        const Radiansf angle = CalculateAngle(vec1, vec2);
        REQUIRE(angle == ToRadians(Degreesf(90)));
    }

    {
        const UnitVector3f vec1(1, 0, 0);
        const UnitVector3f vec2(1, 1, 0);

        const Radiansf angle = CalculateAngle(vec1, vec2);
        REQUIRE(angle == ToRadians(Degreesf(45)));
    }
}


TEST_CASE("Average Point")
{

    constexpr Point2f p1 = Point2f::Zero();
    constexpr Point2f p2 = Point2f::One();

    constexpr Point2f p3 = AveragePoint(p1, p2);

}

TEST_CASE("VectorMath::ToClipCoords")
{
    constexpr Point2i point(0, 50);
    constexpr Vector2ui size(100u, 100u);

    constexpr Point2f clipCoords = ToClipCoords(point, size);

    REQUIRE(clipCoords == Point2f(-1, 0));
}