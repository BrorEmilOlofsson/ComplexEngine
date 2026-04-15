#pragma once
#include "Engine/Utility/Grid/Grid.hpp"
#include "Engine/Math/Shapes/AABB2.hpp"
#include "Engine/Utility/Grid/PrimitiveGrid2.hpp"
#include <ranges>

namespace CLX
{
	template<typename T>
	[[nodiscard]] constexpr T GetVolume(const Vector2<T>& vector) noexcept
	{
		return vector.x * vector.y;
	}

	[[nodiscard]] constexpr bool IsValidGridCoordinates(const Point2i& coords, const Vector2u& gridSize) noexcept
	{
		return IsInside(coords, Point2i::Zero(), Point2i::Zero() + Vector2i(gridSize) - Vector2i::One());
	}

	[[nodiscard]] constexpr int GetIndexByCoordinates(const Point2i& coords, const Vector2u& gridSize) noexcept
	{
		return coords.y * gridSize.x + coords.x;
	}

	[[nodiscard]] constexpr unsigned int GetIndexByCoordinates(const Point2ui& coords, const Vector2u& gridSize) noexcept
	{
		return coords.y * gridSize.x + coords.x;
	}

	[[nodiscard]] constexpr Point2i GetGridCoordinatesByPosition(const Point2f& position, const Point2f& min, const Vector2f& cellSize, const Vector2f& offset) noexcept
	{
		const int xIndex = GetGridCoordinate(position.x, min.x, cellSize.x, offset.x);
		const int yIndex = GetGridCoordinate(position.y, min.y, cellSize.y, offset.y);

		return Point2i(xIndex, yIndex);
	}

	[[nodiscard]] constexpr int GetGridIndexByPosition(const Point2f& position, const Point2f& min, const Vector2f& cellSize, const Vector2f& offset, const Vector2u& gridSize)
	{
		const Point2i coords = GetGridCoordinatesByPosition(position, min, cellSize, offset);
		return GetIndexByCoordinates(coords, gridSize);
    }

	template<std::ranges::range Range>
	[[nodiscard]] constexpr decltype(auto) GetElementByCoordinates(Range& range, const Point2ui& coords, const Vector2u& gridSize)
	{
		return range[GetIndexByCoordinates(coords, gridSize)];
	}

	template<std::ranges::range Range>
	[[nodiscard]] constexpr decltype(auto) GetElementByCoordinates(const Range& range, const Point2ui& coords, const Vector2u& gridSize)
	{
		return range[GetIndexByCoordinates(coords, gridSize)];
	}

	template<typename Func>
	constexpr void ForEachGridCell(const AABB2<unsigned int>& aabb, Func&& function)
	{
		const Point2ui& minCoords = aabb.GetMin();
		const Point2ui& maxCoords = aabb.GetMax();
		for (unsigned int y = minCoords.y; y <= maxCoords.y; ++y)
		{
			for (unsigned int x = minCoords.x; x <= maxCoords.x; ++x)
			{
				function(Point2ui(x, y));
			}
		}
	}

	struct Grid2Types final
	{
		template<typename T>
		using AABB = AABB2<T>;
		template<typename T>
		using Vector = Vector2<T>;
		template<typename T>
		using Point = Point2<T>;

		using PrimitiveGrid = PrimitiveGrid2;
	};

	template<typename T>
	using Grid2 = Grid<T, Grid2Types>;
}