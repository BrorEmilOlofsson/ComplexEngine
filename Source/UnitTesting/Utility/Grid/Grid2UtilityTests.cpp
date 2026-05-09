#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/Utility/Grid/Grid2Utility.hpp"

using namespace CLX;

TEST_CASE("GetPositionRightUnchecked", "[Grid2Utility]")
{
    {
        int position = 0;
        REQUIRE(GetPositionRightUnchecked(position) == 1);
    }

    {
        int position = 5;
        REQUIRE(GetPositionRightUnchecked(position) == 6);
    }
}

TEST_CASE("GetPositionLeftUnchecked", "[Grid2Utility]")
{
    {
        int position = 0;
        REQUIRE(GetPositionLeftUnchecked(position) == -1);
    }
    {
        int position = 5;
        REQUIRE(GetPositionLeftUnchecked(position) == 4);
    }
}

TEST_CASE("GetPositionUpUnchecked", "[Grid2Utility]")
{
    const uint32_t width = 5;
    {
        int position = 0;
        REQUIRE(GetPositionUpUnchecked(position, width) == -5);
    }
    {
        int position = 5;
        REQUIRE(GetPositionUpUnchecked(position, width) == 0);
    }
}

TEST_CASE("GetPositionDownUnchecked", "[Grid2Utility]")
{
    const uint32_t width = 5;
    {
        int position = 0;
        REQUIRE(GetPositionDownUnchecked(position, width) == 5);
    }
    {
        int position = 5;
        REQUIRE(GetPositionDownUnchecked(position, width) == 10);
    }
}

TEST_CASE("GetPositionRight", "[Grid2Utility]")
{
    const uint32_t width = 5;
    {
        int position = 0;
        REQUIRE(GetPositionRight(position, width) == 1);
    }
    {
        int position = 4;
        REQUIRE(GetPositionRight(position, width) == std::nullopt);
    }
}

TEST_CASE("GetPositionLeft", "[Grid2Utility]")
{
    const uint32_t width = 5;
    {
        int position = 0;
        REQUIRE(GetPositionLeft(position, width) == std::nullopt);
    }
    {
        int position = 4;
        REQUIRE(GetPositionLeft(position, width) == 3);
    }
}

TEST_CASE("GetPositionUp", "[Grid2Utility]")
{
    const uint32_t width = 5;
    {
        int position = 0;
        REQUIRE(GetPositionUp(position, width) == std::nullopt);
    }
    {
        int position = 5;
        REQUIRE(GetPositionUp(position, width) == 0);
    }
}

TEST_CASE("GetPositionDown", "[Grid2Utility]")
{
    const uint32_t width = 5;
    const uint32_t height = 5;
    {
        int position = 0;
        REQUIRE(GetPositionDown(position, width, height) == 5);
    }
    {
        int position = 5;
        REQUIRE(GetPositionDown(position, width, height) == 10);
    }
    {
        int position = 23;
        REQUIRE(GetPositionDown(position, width, height) == std::nullopt);
    }
}

TEST_CASE("GetPositionUpLeft")
{
    const uint32_t width = 5;
    {
        int position = 0;
        REQUIRE(GetPositionUpLeft(position, width) == std::nullopt);
    }
    {
        int position = 6;
        REQUIRE(GetPositionUpLeft(position, width) == 0);
    }
}

TEST_CASE("GetPositionUpRight")
{
    const uint32_t width = 5;
    {
        int position = 4;
        REQUIRE(GetPositionUpRight(position, width) == std::nullopt);
    }
    {
        int position = 6;
        REQUIRE(GetPositionUpRight(position, width) == 2);
    }
}

TEST_CASE("GetPositionDownLeft")
{
    const uint32_t width = 5;
    const uint32_t height = 5;
    {
        int position = 0;
        REQUIRE(GetPositionDownLeft(position, width, height) == std::nullopt);
    }
    {
        int position = 6;
        REQUIRE(GetPositionDownLeft(position, width, height) == 10);
    }
}

TEST_CASE("GetPositionDownRight")
{
    const uint32_t width = 5;
    const uint32_t height = 5;
    {
        int position = 4;
        REQUIRE(GetPositionDownRight(position, width, height) == std::nullopt);
    }
    {
        int position = 6;
        REQUIRE(GetPositionDownRight(position, width, height) == 12);
    }
}