#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/Utility/Grid/GridIndexView.hpp"

using namespace CLX;

TEST_CASE("GridIndexRowBasedView can be iterated over", "[GridIndexView]")
{
    auto view = GridIndexRowBasedView(3uz, 2uz);
    std::vector<std::size_t> indices = view | std::ranges::to<std::vector>();
    REQUIRE(indices == std::vector<std::size_t>{0, 1, 2, 3, 4, 5});
}

TEST_CASE("GridIndexColumnBasedView can be iterated over", "[GridIndexView]")
{
    auto view = GridIndexColumnBasedView<std::uint32_t, std::size_t>(3u, 2u);
    std::vector<std::uint32_t> indices = view | std::ranges::to<std::vector>();
    REQUIRE(indices == std::vector<std::uint32_t>{0, 3, 1, 4, 2, 5});
}

TEST_CASE("GridIndexRowBasedOffsetView can be iterated over", "[GridIndexView]")
{
    {
        auto view = GridIndexRowBasedOffsetView(3uz, 2uz, 1uz, 1uz);
        auto indices = view | std::ranges::to<std::vector>();
        REQUIRE(indices == std::vector<std::pair<size_t, size_t>>{{1, 1}, { 1, 2 }, { 1, 3 }, { 2, 1 }, { 2, 2 }, { 2, 3 }});
    }
    {
        auto view = GridIndexRowBasedOffsetView(3uz, 2uz, -1z, -1z);
        std::vector<std::pair<int64_t, int64_t>> indices = view | std::ranges::to<std::vector>();
        REQUIRE(indices == std::vector<std::pair<int64_t, int64_t>>{{-1, -1}, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 0 }, { 0, 1 }});
    }
}