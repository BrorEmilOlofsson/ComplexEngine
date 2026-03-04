#pragma once
#include "Engine/Math/Point2.hpp"
#include "Engine/Math/Point3.hpp"
#include "Engine/Math/Shapes/AABB2.hpp"
#include "Engine/Math/Shapes/AABB3.hpp"
#include "Engine/Math/VectorMath.hpp"
#include "Engine/Utility/Grid/GridCell.hpp"

#include <optional>
#include <vector>
#include <cmath>
#include <unordered_set>
#include <span>

namespace CLX
{
	template<typename T>
	[[nodiscard]] constexpr int GetGridCoordinate(const T position, const T min, const T cellSize, const T offset) noexcept
	{
		const T pos = position - (min - offset);
		const T indexFloat = pos / cellSize;
		return static_cast<int>(Floor(indexFloat));
	}

	template<template<typename> typename AABBType, template<typename> typename PointType, template<typename> typename VectorType>
	constexpr AABBType<unsigned int> GetAABBGridCoordinatesByAABB(const AABBType<float>& aabb, const PointType<float>& min, const VectorType<float>& cellSize, const VectorType<float>& offset, const VectorType<unsigned int>& gridSize)
	{
		const PointType<int> minCoords = GetGridCoordinatesByPosition(aabb.GetMin(), min, cellSize, offset);
		const PointType<int> maxCoords = GetGridCoordinatesByPosition(aabb.GetMax(), min, cellSize, offset);

		const PointType<int> clampedMinCoords = Max(minCoords, PointType<int>::Zero());
		const PointType<int> clampedMaxCoords = Min(maxCoords, PointType<int>::Zero() + VectorType<int>(gridSize) - VectorType<int>::One());

		return AABBType<unsigned int>::FromMinAndMax(PointType<unsigned int>(clampedMinCoords), PointType<unsigned int>(clampedMaxCoords));
	}

	template<typename T, typename Types>
	class Grid final
	{
	public:

		template<typename U>
		using AABBType = typename Types::template AABB<U>;

		template<typename U>
		using VectorType = typename Types::template Vector<U>;

		template<typename U>
		using PointType = typename Types::template Point<U>;

		using PrimitiveGridType = typename Types::PrimitiveGrid;

		using VectorTypef = VectorType<float>;
		using VectorTypeui = VectorType<unsigned int>;
		using PointTypef = PointType<float>;
		using PointTypei = PointType<int>;
		using PointTypeui = PointType<unsigned int>;
		using AABBTypef = AABBType<float>;
		using AABBTypeui = AABBType<unsigned int>;

		constexpr Grid() = default;
		constexpr Grid(const VectorTypeui& gridSize, const VectorTypef& cellSize, const PointTypef& minPos, const VectorTypef& offset = VectorTypef::Zero());
		constexpr Grid(const PrimitiveGridType& primitiveGrid);

		[[nodiscard]] constexpr std::optional<GridCell<T>*> GetCellByPosition(const PointTypef& position);
		[[nodiscard]] constexpr std::optional<const GridCell<T>*> GetCellByPosition(const PointTypef& position) const;

		[[nodiscard]] constexpr const VectorTypef& GetCellSize() const noexcept;
		[[nodiscard]] constexpr const VectorTypeui& GetGridSize() const noexcept;
		[[nodiscard]] constexpr const PointTypef& GetMinPosition() const noexcept;
		[[nodiscard]] constexpr const VectorTypef& GetOffset() const noexcept;

		constexpr void AddObjectToCellsInsideAABB(const T& object, const AABBTypef& aabb);
		[[nodiscard]] constexpr std::unordered_set<T> GetObjectsByAABB(const AABBTypef& aabb) const;
		[[nodiscard]] constexpr std::span<const GridCell<T>> GetCells() const noexcept;

		constexpr void Add(const PointTypef& position, const T& object);
		constexpr void Add(const PointTypef& position, T&& object);
		constexpr void Clear();

		[[nodiscard]] constexpr PrimitiveGridType GetPrimitiveGrid() const noexcept;

	private:

		constexpr std::vector<const GridCell<T>*> GetCellsByAABB(const AABBTypef& aabb) const;
		constexpr GridCell<T>& GetCellByCoordinates(const PointTypeui& coords) noexcept;
		constexpr const GridCell<T>& GetCellByCoordinates(const PointTypeui& coords) const noexcept;

	private:

		std::vector<GridCell<T>> mCells;
		VectorTypeui mGridSize;
		VectorTypef mCellSize = VectorTypef::One();
		PointTypef mMinPos;
		VectorTypef mOffset = VectorTypef::Zero();
	};

	template<typename T, typename Types>
	constexpr Grid<T, Types>::Grid(const VectorTypeui& gridSize, const VectorTypef& cellSize, const PointTypef& minPos, const VectorTypef& offset)
		: mCells(static_cast<std::size_t>(GetVolume(gridSize)))
		, mGridSize(gridSize)
		, mCellSize(cellSize)
		, mMinPos(minPos)
		, mOffset(offset)
	{
	}

	template<typename T, typename Types>
	constexpr Grid<T, Types>::Grid(const PrimitiveGridType& primitiveGrid)
		: Grid<T, Types>(primitiveGrid.gridSize, primitiveGrid.cellSize, primitiveGrid.minPos, primitiveGrid.offset)
	{
	}

	template<typename T, typename Types>
	constexpr std::optional<GridCell<T>*> Grid<T, Types>::GetCellByPosition(const PointTypef& position)
	{
		const PointTypei coords = GetGridCoordinatesByPosition(position, mMinPos, mCellSize, mOffset);

		if (!IsValidGridCoordinates(coords, mGridSize))
		{
			return std::nullopt;
		}

		return &GetCellByCoordinates(PointTypeui(coords));
	}

	template<typename T, typename Types>
	constexpr std::optional<const GridCell<T>*> Grid<T, Types>::GetCellByPosition(const PointTypef& position) const
	{
		const PointTypei coords = GetGridCoordinatesByPosition(position, mMinPos, mCellSize, mOffset);

		if (!IsValidGridCoordinates(coords, mGridSize))
		{
			return std::nullopt;
		}

		return &GetCellByCoordinates(PointTypeui(coords));
	}

	template<typename T, typename Types>
	constexpr const typename Grid<T, Types>::VectorTypef& Grid<T, Types>::GetCellSize() const noexcept
	{
		return mCellSize;
	}

	template<typename T, typename Types>
	constexpr const typename Grid<T, Types>::VectorTypeui& Grid<T, Types>::GetGridSize() const noexcept
	{
		return mGridSize;
	}

	template<typename T, typename Types>
	constexpr const typename Grid<T, Types>::PointTypef& Grid<T, Types>::GetMinPosition() const noexcept
	{
		return mMinPos;
	}

	template<typename T, typename Types>
	constexpr const typename Grid<T, Types>::VectorTypef& Grid<T, Types>::GetOffset() const noexcept
	{
		return mOffset;
	}

	template<typename T, typename Types>
	constexpr void Grid<T, Types>::AddObjectToCellsInsideAABB(const T& aObject, const AABBTypef& aabb)
	{
		const AABBTypeui aabbui = GetAABBGridCoordinatesByAABB<AABBType, PointType, VectorType>(aabb, mMinPos, mCellSize, mOffset, mGridSize);

		ForEachGridCell(aabbui, [this, &aObject](const PointTypeui& coords)
			{
				GetCellByCoordinates(coords).Add(aObject);
			});

	}

	template<typename T, typename Types>
	constexpr std::vector<const GridCell<T>*> Grid<T, Types>::GetCellsByAABB(const AABBTypef& aabb) const
	{
		const AABBTypeui aabbui = GetAABBGridCoordinatesByAABB<AABBType, PointType, VectorType>(aabb, mMinPos, mCellSize, mOffset, mGridSize);

		const VectorTypeui extent = aabbui.GetExtent();

		std::vector<const GridCell<T>*> cells;
		cells.reserve(static_cast<std::size_t>(GetVolume(extent)));

		ForEachGridCell(aabbui, [&cells, this](const PointTypeui& coords)
			{
				const GridCell<T>& cell = GetCellByCoordinates(coords);
				cells.push_back(&cell);
			});

		return cells;
	}

	template<typename T, typename Types>
	constexpr std::unordered_set<T> Grid<T, Types>::GetObjectsByAABB(const AABBTypef& aabb) const
	{
		std::unordered_set<T> objects;

		const std::vector<const GridCell<T>*> cells = GetCellsByAABB(aabb);
		for (const GridCell<T>* cell : cells)
		{
			const auto& objectsInCell = cell->GetObjects();
			objects.insert(objectsInCell.begin(), objectsInCell.end());
		}
		return objects;
	}

	template<typename T, typename Types>
	constexpr std::span<const GridCell<T>> Grid<T, Types>::GetCells() const noexcept
	{
		return mCells;
	}

	template<typename T, typename Types>
	constexpr void Grid<T, Types>::Add(const PointTypef& position, const T& object)
	{
		auto cell = GetCellByPosition(position);
		if (cell)
		{
			cell.value()->Add(object);
		}
	}

	template<typename T, typename Types>
	constexpr void Grid<T, Types>::Add(const PointTypef& position, T&& object)
	{
		auto cell = GetCellByPosition(position);
		if (cell)
		{
			cell.value()->Add(std::move(object));
		}
	}

	template<typename T, typename Types>
	constexpr void Grid<T, Types>::Clear()
	{
		for (GridCell<T>& cell : mCells)
		{
			cell.Clear();
		}
	}

	template<typename T, typename Types>
	constexpr Grid<T, Types>::PrimitiveGridType Grid<T, Types>::GetPrimitiveGrid() const noexcept
	{
		return PrimitiveGridType
		{
			.gridSize = mGridSize,
			.cellSize = mCellSize,
			.minPos = mMinPos,
			.offset = mOffset,
		};
	}

	template<typename T, typename Types>
	constexpr GridCell<T>& Grid<T, Types>::GetCellByCoordinates(const PointTypeui& coords) noexcept
	{
		return GetElementByCoordinates(mCells, coords, mGridSize);
	}

	template<typename T, typename Types>
	constexpr const GridCell<T>& Grid<T, Types>::GetCellByCoordinates(const PointTypeui& coords) const noexcept
	{
		return GetElementByCoordinates(mCells, coords, mGridSize);
	}
}

