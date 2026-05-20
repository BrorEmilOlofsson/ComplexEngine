#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/Utility/Grid/GridIndexView.hpp"

using namespace CLX;

TEST_CASE("GridIndexRowBasedView can be iterated over", "[GridIndexView]")
{
    std::vector<std::size_t> indices = GridIndexRowBasedView(3u, 2u) | std::ranges::to<std::vector>();
    REQUIRE(indices == std::vector<std::size_t>{0, 1, 2, 3, 4, 5});
}

TEST_CASE("GridIndexColumnBasedView can be iterated over", "[GridIndexView]")
{
    std::vector<std::uint32_t> indices = GridIndexColumnBasedView<std::uint32_t, std::size_t>(3u, 2u) | std::ranges::to<std::vector>();
    REQUIRE(indices == std::vector<std::uint32_t>{0, 3, 1, 4, 2, 5});
}