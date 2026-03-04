#pragma once
#include "Engine/Utility/Grid/Grid.hpp"
#include "Engine/Math/Shapes/AABB3.hpp"
#include "Engine/Utility/Grid/PrimitiveGrid3.hpp"

namespace Simple
{

	template<typename T>
	[[nodiscard]] constexpr T GetVolume(const Vector3<T>& vector) noexcept
	{
		return vector.x * vector.y * vector.z;
	}

	[[nodiscard]] constexpr bool IsValidGridCoordinates(const Point3i& coords, const Vector3ui& gridSize) noexcept
	{
		return IsInside(coords, Point3i::Zero(), Point3i::Zero() + Vector3i(gridSize) - Vector3i::One());
	}

	[[nodiscard]] constexpr Point3i GetGridCoordinatesByPosition(const Point3f& position, const Point3f& min, const Vector3f& cellSize, const Vector3f& offset) noexcept
	{
		const int xIndex = GetGridCoordinate(position.x, min.x, cellSize.x, offset.x);
		const int yIndex = GetGridCoordinate(position.y, min.y, cellSize.y, offset.y);
		const int zIndex = GetGridCoordinate(position.z, min.z, cellSize.z, offset.z);

		return Point3i(xIndex, yIndex, zIndex);
	}

	[[nodiscard]] constexpr unsigned int GetIndexByCoordinates(const Point3ui& coords, const Vector3ui& gridSize) noexcept
	{
		return coords.z * gridSize.x * gridSize.y + coords.y * gridSize.x + coords.x;
	}

	template<std::ranges::range Range>
	[[nodiscard]] constexpr decltype(auto) GetElementByCoordinates(Range& range, const Point3ui& coords, const Vector3ui& gridSize)
	{
		return range[GetIndexByCoordinates(coords, gridSize)];
	}

	template<std::ranges::range Range>
	[[nodiscard]] constexpr decltype(auto) GetElementByCoordinates(const Range& range, const Point3ui& coords, const Vector3ui& gridSize)
	{
		return range[GetIndexByCoordinates(coords, gridSize)];
	}

	template<typename Func>
	constexpr void ForEachGridCell(const AABB3<unsigned int>& aabb, Func&& function)
	{
		const Point3ui& minCoords = aabb.GetMin();
		const Point3ui& maxCoords = aabb.GetMax();
		for (unsigned int z = minCoords.z; z <= maxCoords.z; ++z)
		{
			for (unsigned int y = minCoords.y; y <= maxCoords.y; ++y)
			{
				for (unsigned int x = minCoords.x; x <= maxCoords.x; ++x)
				{
					function(Point3ui(x, y, z));
				}
			}
		}
	}

	struct Grid3Types final
	{
		template<typename T>
		using AABB = AABB3<T>;
		template<typename T>
		using Vector = Vector3<T>;
		template<typename T>
		using Point = Point3<T>;

		using PrimitiveGrid = PrimitiveGrid3;
	};

	template<typename T>
	using Grid3 = Grid<T, Grid3Types>;

}