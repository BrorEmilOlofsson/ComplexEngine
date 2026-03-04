#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/Utility/Grid/Grid2.hpp"
#include "Engine/Utility/Grid/Grid3.hpp"

using namespace Simple;

TEST_CASE("Grid2 Coordinates")
{
	{
		constexpr Point2i c = GetGridCoordinatesByPosition(Point2f(2.5f, 3.5f), Point2f(0.0f, 0.0f),
			Vector2f(1.0f, 1.0f), Vector2f(0.0f, 0.0f));

		REQUIRE(c == Point2i(2, 3));
	}
	{
		constexpr Point2i c = GetGridCoordinatesByPosition(Point2f(2.5f, 3.5f), Point2f(0.0f, 0.0f),
			Vector2f(1.0f, 1.0f), Vector2f(0.5f, 0.5f));

		REQUIRE(c == Point2i(3, 4));
	}

	{
		constexpr Point2i c = GetGridCoordinatesByPosition(Point2f(145.5f, 23.5f), Point2f(2.0f, 2.0f),
			Vector2f(5.0f, 5.0f), Vector2f(0.0f, 0.0f));

		REQUIRE(c == Point2i(28, 4));
	}
}

TEST_CASE("Grid2 IsValidCoordinates")
{
	constexpr Vector2ui gridSize(10, 10);
	REQUIRE(IsValidGridCoordinates(Point2i(0, 0), gridSize));
	REQUIRE(IsValidGridCoordinates(Point2i(9, 9), gridSize));
	REQUIRE(!IsValidGridCoordinates(Point2i(-1, 0), gridSize));
	REQUIRE(!IsValidGridCoordinates(Point2i(10, 0), gridSize));
	REQUIRE(!IsValidGridCoordinates(Point2i(0, -1), gridSize));
	REQUIRE(!IsValidGridCoordinates(Point2i(0, 10), gridSize));
}

TEST_CASE("Grid2 Add")
{
	Grid2<std::string> grid(Vector2ui(10, 10), Vector2f(1.0f, 1.0f), Point2f(0.0f, 0.0f));
	
	constexpr std::string_view str = "TestObject1";
	grid.Add(Point2f(1.5f, 6.5f), std::string(str));

	auto cell = grid.GetCellByPosition(Point2f(1.5f, 6.5f));

	REQUIRE(cell.has_value());
	REQUIRE(cell.value()->GetObjects().size() == 1);
	REQUIRE(cell.value()->GetObjects().find(std::string(str)) != cell.value()->GetObjects().end());
}

TEST_CASE("Grid3 GetCellsByAABB")
{
	Grid3<std::string> grid(Vector3ui(10, 10, 10), Vector3f(1.0f, 1.0f, 1.0f), Point3f(0.0f, 0.0f, 0.f));

	constexpr std::string_view str = "TestObject1";
	grid.Add(Point3f(1.5f, 6.5f, 2.58f), std::string(str));
	const auto cell = grid.GetCellByPosition(Point3f(1.5f, 6.5f, 2.58f));

	REQUIRE(cell.has_value());
	REQUIRE(cell.value()->GetObjects().size() == 1);
	REQUIRE(cell.value()->GetObjects().find(std::string(str)) != cell.value()->GetObjects().end());
}