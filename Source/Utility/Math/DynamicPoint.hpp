#pragma once
#include <vector>
#include <stdexcept>
#include "Utility/Math/DynamicVector.hpp"

namespace Simple
{

	template<typename T>
	class DynamicPoint final
	{
	public:

		using type = T;

		constexpr DynamicPoint() = default;

		constexpr explicit DynamicPoint(const std::size_t dimensionCount)
			: mValues(dimensionCount)
		{
		}

		constexpr explicit DynamicPoint(std::initializer_list<T> list)
			: mValues(list)
		{
		}
		
		[[nodiscard]] constexpr T& operator[](const std::size_t index)
		{
			return mValues[index];
		}

		[[nodiscard]] constexpr const T& operator[](const std::size_t index) const
		{
			return mValues[index];
		}

		[[nodiscard]] constexpr std::size_t GetDimensionCount() const noexcept
		{
			return size(mValues);
		}

		constexpr void SetDimensionCount(const std::size_t dimensionCount)
		{
			mValues.resize(dimensionCount);
		}

		constexpr void AddDimension(const T& value)
		{
			mValues.push_back(value);
		}

		constexpr void AddDimension(T&& value)
		{
			mValues.push_back(std::move(value));
		}

		friend constexpr bool operator==(const DynamicPoint& a, const DynamicPoint& b) noexcept;

	private:

		std::vector<T> mValues;
	};

	using DynamicPointf = DynamicPoint<float>;
	using DynamicPointd = DynamicPoint<double>;

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const DynamicPoint<T>& a, const DynamicPoint<T>& b) noexcept
	{
		if (a.GetDimensionCount() != b.GetDimensionCount())
		{
			return false;
		}
		return a.mValues == b.mValues;
	}

	template<typename T, typename Operation>
	[[nodiscard]] constexpr DynamicPoint<T> PerformOperation(const DynamicPoint<T>& point, const DynamicVector<T>& vector, Operation&& operation)
	{
		const std::size_t dimensionCount = point.GetDimensionCount();
		if (dimensionCount != vector.GetDimensionCount())
		{
			throw std::invalid_argument("Dimensions are not the same");
		}

		DynamicPoint<T> result;
		result.SetDimensionCount(dimensionCount);

		for (std::size_t i = 0; i < dimensionCount; i++)
		{
			result[i] = operation(point[i], vector[i]);
		}

		return result;
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicPoint<T> operator+(const DynamicPoint<T>& point, const DynamicVector<T>& vector)
	{
		return PerformOperation(point, vector, std::plus{});
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicPoint<T> operator-(const DynamicPoint<T>& point, const DynamicVector<T>& vector)
	{
		return PerformOperation(point, vector, std::minus{});
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicPoint<T>& operator+=(DynamicPoint<T>& point, const DynamicVector<T>& vector)
	{
		point = point + vector;
		return point;
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicPoint<T>& operator-=(DynamicPoint<T>& point, const DynamicVector<T>& vector)
	{
		point = point - vector;
		return point;
	}

	template<typename T> 
	[[nodiscard]] constexpr DynamicVector<T> operator-(const DynamicPoint<T>& a, const DynamicPoint<T>& b)
	{
		const std::size_t dimensionCount = a.GetDimensionCount();
		if (dimensionCount != b.GetDimensionCount())
		{
			throw std::invalid_argument("Dimensions are not the same");
		}

		DynamicVector<T> result;
		result.SetDimensionCount(dimensionCount);

		for (std::size_t i = 0; i < dimensionCount; i++)
		{
			result[i] = a[i] - b[i];
		}

		return result;
	}
}