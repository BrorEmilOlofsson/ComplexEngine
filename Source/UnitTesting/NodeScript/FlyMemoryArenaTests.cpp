#include <External/Catch2/catch_amalgamated.hpp>
#include "NodeScript/Memory/FlyMemoryArena.hpp"
#include <print>

TEST_CASE("NodeScript Testing")
{

    Fly::MemoryArena<40> arena;

    bool& b = arena.Allocate<bool>(true);
    std::string& str = arena.Allocate<std::string>("Hello, World!");

    int& intRef = arena.Allocate<int>(42);
    double& doubleRef = arena.Allocate<double>(3.14);

    REQUIRE(b);
    REQUIRE(str == "Hello, World!");
    REQUIRE(intRef == 42);
    REQUIRE(doubleRef == 3.14);


}