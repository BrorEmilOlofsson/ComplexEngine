#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Math/RotationMatrix2.hpp"
#include "Engine/Utility/Approximation.hpp"

using namespace Simple;

TEST_CASE("RotationMatrix2::Default Constructor", "[RotationMatrix2]")
{
    RotationMatrix2d rotMat;
    REQUIRE(rotMat.GetRight() == UnitVector2d::Right());
    REQUIRE(rotMat.GetUp() == UnitVector2d::Up());
}

TEST_CASE("RotationMatrix2::FromValues", "[RotationMatrix2]")
{
    std::array<double, 4> values = { 0.0, 1.0, -1.0, 0.0 };
    RotationMatrix2d rotMat = RotationMatrix2d::FromValues(values);
    REQUIRE(rotMat.GetRight() == UnitVector2d(0.0, 1.0));
    REQUIRE(rotMat.GetUp() == UnitVector2d(-1.0, 0.0));
}

TEST_CASE("RotationMatrix2::FromX", "[RotationMatrix2]")
{
    UnitVector2d xAxis(0.0, -1.0);
    RotationMatrix2d rotMat = RotationMatrix2d::FromX(xAxis);
    REQUIRE(rotMat.GetRight() == xAxis);
    REQUIRE(rotMat.GetUp() == UnitVector2d(1.0, 0.0));
}

TEST_CASE("RotationMatrix2::FromY", "[RotationMatrix2]")
{
    UnitVector2d yAxis(0.0, -1.0);
    RotationMatrix2d rotMat = RotationMatrix2d::FromY(yAxis);
    REQUIRE(rotMat.GetRight() == UnitVector2d(-1.0, 0.0));
    REQUIRE(rotMat.GetUp() == yAxis);
}

TEST_CASE("RotationMatrix2::Operator[]", "[RotationMatrix2]")
{
    std::array<double, 4> values = { 0.0, 1.0, -1.0, 0.0 };
    RotationMatrix2d rotMat = RotationMatrix2d::FromValues(values);

    REQUIRE(rotMat[0] == 0.0);
    REQUIRE(rotMat[1] == 1.0);
    REQUIRE(rotMat[2] == -1.0);
    REQUIRE(rotMat[3] == 0.0);
}

TEST_CASE("RotationMatrix2::Multiplication Operator (Vector2 * RotationMatrix2)", "[RotationMatrix]")
{
    SECTION("90 Degree Rotation - Vector right")
    {
        constexpr Vector2d vec(1.0, 0.0);
        constexpr RotationMatrix2d rotMat = RotationMatrix2d::FromX(UnitVector2d::Up());
        const Vector2d result = vec * rotMat;
        REQUIRE(result.x == Approx(0.0));
        REQUIRE(result.y == Approx(1.0));
    }
    SECTION("270 Degree Rotation - Vector right")
    {
        constexpr Vector2d vec(1.0, 0.0);
        constexpr RotationMatrix2d rotMat = RotationMatrix2d::FromX(UnitVector2d::Down());
        const Vector2d result = vec * rotMat;
        REQUIRE(result.x == Approx(0.0));
        REQUIRE(result.y == Approx(-1.0));
    }
    SECTION("90 Degree Rotation - Vector left")
    {
        constexpr Vector2d vec(-1.0, 0.0);
        constexpr RotationMatrix2d rotMat = RotationMatrix2d::FromX(UnitVector2d::Up());
        const Vector2d result = vec * rotMat;
        REQUIRE(result.x == Approx(0.0));
        REQUIRE(result.y == Approx(-1.0));
    }
    SECTION("270 Degree Rotation - Vector left")
    {
        constexpr Vector2d vec(-1.0, 0.0);
        constexpr RotationMatrix2d rotMat = RotationMatrix2d::FromX(UnitVector2d::Down());
        const Vector2d result = vec * rotMat;
        REQUIRE(result.x == Approx(0.0));
        REQUIRE(result.y == Approx(1.0));
    }
}

TEST_CASE("RotationMatrix::Equality Operator", "[RotationMatrix2]")
{
    RotationMatrix2d rotMat1 = RotationMatrix2d::FromValues({ 1.0, 0.0, 0.0, 1.0 });
    RotationMatrix2d rotMat2 = RotationMatrix2d::FromValues({ 1.0, 0.0, 0.0, 1.0 });
    RotationMatrix2d rotMat3 = RotationMatrix2d::FromValues({ 0.0, 1.0, -1.0, 0.0 });
    REQUIRE(rotMat1 == rotMat2);
    REQUIRE_FALSE(rotMat1 == rotMat3);
}