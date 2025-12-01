#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/Math/Vector3.hpp"

using namespace Simple;

TEST_CASE("Vector3 Default Construction", "[Vector3]")
{
    const Vector3f vecf;
    REQUIRE(vecf.x == 0.0f);
    REQUIRE(vecf.y == 0.0f);
    REQUIRE(vecf.z == 0.0f);
    const Vector3d vecd;
    REQUIRE(vecd.x == 0.0);
    REQUIRE(vecd.y == 0.0);
    REQUIRE(vecd.z == 0.0);
    const Vector3i veci;
    REQUIRE(veci.x == 0);
    REQUIRE(veci.y == 0);
    REQUIRE(veci.z == 0);
}

TEST_CASE("Vector3 Construction", "[Vector3]")
{
    const Vector3f vec1(3.0f, 4.0f, 5.0f);
    REQUIRE(vec1.x == 3.0f);
    REQUIRE(vec1.y == 4.0f);
    REQUIRE(vec1.z == 5.0f);
    const Vector3d vec2(5.0, 6.0, 7.0);
    REQUIRE(vec2.x == 5.0);
    REQUIRE(vec2.y == 6.0);
    REQUIRE(vec2.z == 7.0);
    const Vector3i vec3(7, 8, 9);
    REQUIRE(vec3.x == 7);
    REQUIRE(vec3.y == 8);
    REQUIRE(vec3.z == 9);
}

TEST_CASE("Vector3 Constants", "[Vector3]")
{
    const Vector3f zero = Vector3f::Zero();
    const Vector3f one = Vector3f::One();
    REQUIRE(zero == Vector3f(0.0f, 0.0f, 0.0f));
    REQUIRE(one == Vector3f(1.0f, 1.0f, 1.0f));
}

TEST_CASE("Vector3 Equality Operator", "[Vector3]")
{
    const Vector3i a(1, 2, 3);
    const Vector3i b(1, 2, 3);
    const Vector3i c(2, 3, 4);
    REQUIRE(a == b);
    REQUIRE(a != c);
}

TEST_CASE("Vector3 Type Conversion", "[Vector3]")
{
    const Vector3i vecInt(3, 4, 5);
    const Vector3f vecFloat = static_cast<Vector3f>(vecInt);
    REQUIRE(vecFloat.x == 3.0f);
    REQUIRE(vecFloat.y == 4.0f);
    REQUIRE(vecFloat.z == 5.0f);
    const Vector3d vecDouble = static_cast<Vector3d>(vecInt);
    REQUIRE(vecDouble.x == 3.0);
    REQUIRE(vecDouble.y == 4.0);
    REQUIRE(vecDouble.z == 5.0);
}

TEST_CASE("Vector3 addition", "[Vector3]")
{
    const Vector3f a(1, 2, 3);
    const Vector3f b(4, 5, 6);
    const Vector3f result = a + b;

    REQUIRE(result.x == 5);
    REQUIRE(result.y == 7);
    REQUIRE(result.z == 9);
}

TEST_CASE("Vector3 subtraction", "[Vector3]")
{
    const Vector3f a(11, 2, -13);
    const Vector3f b(4, 5, 6);
    const Vector3f result = a - b;

    REQUIRE(result.x == 7);
    REQUIRE(result.y == -3);
    REQUIRE(result.z == -19);
}

TEST_CASE("Vector3 multiplication", "[Vector3]")
{
    const Vector3f a(11, 2, -13);
    const Vector3f b(4, 5, 6);
    const Vector3f result = a * b;

    REQUIRE(result.x == 44);
    REQUIRE(result.y == 10);
    REQUIRE(result.z == -78);
}

TEST_CASE("Vector3 division", "[Vector3]")
{
    const Vector3f a(11, 2, -13);
    const Vector3f b(4, 5, 6);
    const Vector3f result = a / b;

    REQUIRE(result.x == 11.f / 4);
    REQUIRE(result.y == 0.4f);
    REQUIRE(result.z == -13.f / 6.f);
}

TEST_CASE("Vector3 addition assignment", "[Vector3]")
{
    Vector3f a(1, 2, 3);
    const Vector3f b(4, 5, 6);
    a += b;
    REQUIRE(a.x == 5);
    REQUIRE(a.y == 7);
    REQUIRE(a.z == 9);
}

TEST_CASE("Vector3 subtraction assignment", "[Vector3]")
{
    Vector3f a(11, 2, -13);
    const Vector3f b(4, 5, 6);
    a -= b;
    REQUIRE(a.x == 7);
    REQUIRE(a.y == -3);
    REQUIRE(a.z == -19);
}

TEST_CASE("Vector3 multiplication assignment", "[Vector3]")
{
    Vector3f a(11, 2, -13);
    const float scalar = 3.0f;
    a *= scalar;
    REQUIRE(a.x == 33);
    REQUIRE(a.y == 6);
    REQUIRE(a.z == -39);
}

TEST_CASE("Vector3 division assignment", "[Vector3]")
{
    Vector3f a(11, 2, -13);
    const float scalar = 2.0f;
    a /= scalar;
    REQUIRE(a.x == 5.5f);
    REQUIRE(a.y == 1.0f);
    REQUIRE(a.z == -6.5f);
}

TEST_CASE("Vector3 negation", "[Vector3]")
{
    const Vector3f vec(1.0f, -2.0f, 3.0f);
    const Vector3f negated = -vec;
    REQUIRE(negated.x == -1.0f);
    REQUIRE(negated.y == 2.0f);
    REQUIRE(negated.z == -3.0f);
}

TEST_CASE("Vector3 scalar multiplication", "[Vector3]")
{
    const Vector3f vec(2.0f, -3.0f, 4.0f);
    const float scalar = 3.0f;
    const Vector3f result = vec * scalar;
    REQUIRE(result.x == 6.0f);
    REQUIRE(result.y == -9.0f);
    REQUIRE(result.z == 12.0f);
}

TEST_CASE("Vector3 scalar division", "[Vector3]")
{
    const Vector3f vec(6.0f, -9.0f, 12.0f);
    const float scalar = 3.0f;
    const Vector3f result = vec / scalar;
    REQUIRE(result.x == 2.0f);
    REQUIRE(result.y == -3.0f);
    REQUIRE(result.z == 4.0f);
}