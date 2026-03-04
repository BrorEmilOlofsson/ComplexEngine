#pragma once
#include <vector>
#include <optional>
#include "AIMath.hpp"

namespace CLX
{

	template<typename T>
	class AIDynamicGridCell final
	{
	public:

		constexpr AIDynamicGridCell() = default;


		constexpr void Clear()
		{
			myObjects.clear();
		}

		constexpr void AddObject(const T& aObject)
		{
			myObjects.push_back(aObject);
		}

		constexpr void AddObject(T&& aObject)
		{
			myObjects.push_back(std::forward<T>(aObject));
		}

	private:

		std::vector<T> myObjects;
	};

	template<typename T>
	class AIDynamicGrid
	{
		using GridCoordinates = std::optional<DynamicVectorui>;
	public:

		constexpr AIDynamicGrid() = default;
		constexpr AIDynamicGrid(const DynamicVectorui& aGridSize, const DynamicVectorf& aCellSize, const DynamicPointf& aStartPoint);

		constexpr std::optional<const AIDynamicGridCell<T>*> GetCellByPoint(const DynamicPointf& aPoint) const;
		constexpr std::optional<AIDynamicGridCell<T>*> GetCellByPoint(const DynamicPointf& aPoint);

		constexpr const DynamicVectorf& GetCellSize() const;
		constexpr const DynamicVectorui& GetGridSize() const;
		constexpr const DynamicPointf& GetStartPoint() const;

		constexpr void Clear();

	private:

		constexpr GridCoordinates GetCoordinatesByPoint(const DynamicPointf& aPoint) const;
		constexpr AIDynamicGridCell<T>* GetCellByCoordinates(const DynamicVectorui& aCoords);
		constexpr const AIDynamicGridCell<T>* GetCellByCoordinates(const DynamicVectorui& aCoords) const;
		constexpr unsigned int GetIndexByCoordinates(const DynamicVectorui& aCoords) const;

	private:

		std::vector<AIDynamicGridCell<T>> myCells;
		DynamicVectorui myGridSize;
		DynamicVectorf myCellSize;
		DynamicPointf mStartPoint;
	};

	template<typename T>
	constexpr AIDynamicGrid<T>::AIDynamicGrid(const DynamicVectorui& aGridSize, const DynamicVectorf& aCellSize, const DynamicPointf& aStartPoint)
		: myGridSize(aGridSize)
		, myCellSize(aCellSize)
		, mStartPoint(aStartPoint)
	{
		size_t size = 1;
		for (size_t i = 0; i < aGridSize.GetDimensionCount(); i++)
		{
			size *= aGridSize[i];
		}
		myCells.resize(size);
	}

	template<typename T>
	constexpr std::optional<const AIDynamicGridCell<T>*> AIDynamicGrid<T>::GetCellByPoint(const DynamicPointf& aPoint) const
	{
		const GridCoordinates coords = GetCoordinatesByPoint(aPoint);

		if (!coords)
		{
			return nullptr;
		}

		return GetCellByCoordinates(coords.value());
	}

	template<typename T>
	constexpr std::optional<AIDynamicGridCell<T>*> AIDynamicGrid<T>::GetCellByPoint(const DynamicPointf& aPoint)
	{
		const GridCoordinates coords = GetCoordinatesByPoint(aPoint);

		if (!coords)
		{
			return std::nullopt;
		}

		return GetCellByCoordinates(coords.value());
	}

	template<typename T>
	constexpr const DynamicVectorf& AIDynamicGrid<T>::GetCellSize() const
	{
		return myCellSize;
	}

	template<typename T>
	constexpr const DynamicVectorui& AIDynamicGrid<T>::GetGridSize() const
	{
		return myGridSize;
	}

	template<typename T>
	constexpr const DynamicPointf& AIDynamicGrid<T>::GetStartPoint() const
	{
		return mStartPoint;
	}

	template<typename T>
	constexpr void AIDynamicGrid<T>::Clear()
	{
		for (AIDynamicGridCell<T>& cell : myCells)
		{
			cell.Clear();
		}
	}

	template<typename T>
	constexpr typename AIDynamicGrid<T>::GridCoordinates AIDynamicGrid<T>::GetCoordinatesByPoint(const DynamicPointf& aPoint) const
	{
		DynamicVectorui result(aPoint.GetDimensionCount());
		for (size_t i = 0; i < aPoint.GetDimensionCount(); i++)
		{
			const float point = aPoint[i] - mStartPoint[i];
			const int index = static_cast<int>(std::floor(point / myCellSize[i]));

			if (index >= static_cast<int>(myGridSize[i]) || index < 0)
			{
				return std::nullopt;
			}

			result[i] = static_cast<unsigned int>(index);
		}

		return result;
	}

	template<typename T>
	constexpr AIDynamicGridCell<T>* AIDynamicGrid<T>::GetCellByCoordinates(const DynamicVectorui& aCoords)
	{
		return &myCells[GetIndexByCoordinates(aCoords)];
	}

	template<typename T>
	constexpr const AIDynamicGridCell<T>* AIDynamicGrid<T>::GetCellByCoordinates(const DynamicVectorui& aCoords) const
	{
		return &myCells[GetIndexByCoordinates(aCoords)];
	}

	template<typename T>
	constexpr unsigned int AIDynamicGrid<T>::GetIndexByCoordinates(const DynamicVectorui& aCoords) const
	{
		unsigned int result = 0;
		unsigned int cumulativeProduct = 1;
		const size_t dimensionCount = aCoords.GetDimensionCount();
		for (size_t i = 0; i < dimensionCount; i++)
		{
			/*unsigned int tempResult = aCoords[i];
			for (size_t j = 0; j < i; j++)
			{
				tempResult *= myGridSize[j];
			}
			result += tempResult;*/

			result += aCoords[i] * cumulativeProduct;
			cumulativeProduct *= myGridSize[i];
		}
		return result;
	}
}