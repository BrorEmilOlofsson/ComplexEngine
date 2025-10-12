#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include "Utility/Math/DynamicVector.hpp"

namespace Simple
{

	template<std::ranges::range R>
	[[nodiscard]] constexpr auto LengthSquared(const R& range)
	{
		using T = std::ranges::range_value_t<R>;
		return std::accumulate(std::ranges::begin(range), std::ranges::end(range), T{ 0 },
			[](const T acc, const T& val)
			{
				return acc + val * val;
			});
	}

	template<std::ranges::range R>
	[[nodiscard]] constexpr auto Length(const R& range)
	{
		return Sqrt(LengthSquared(range));
	}

	template<bool ThrowError, std::ranges::range R>
	[[nodiscard]] constexpr void Normalize(R& range)
	{
		using T = std::ranges::range_value_t<R>;
		const T lengthSquared = LengthSquared(range);
		if (lengthSquared == T{ 0 })
		{
			if constexpr (ThrowError)
			{
				throw std::runtime_error("Cannot normalize zero vector");
			}
			else
			{
				return;
			}
		}
		else if (lengthSquared == static_cast<T>(1.0))
		{
			return;
		}
		else
		{
			const T length = Sqrt(lengthSquared);
			std::ranges::for_each(range, [length](auto& v)
				{
					v /= length;
				});
		}
	}

	template<typename T>
	class DynamicUnitVector final
	{
	public:

		using type = T;

		constexpr DynamicUnitVector() = default;

		constexpr explicit DynamicUnitVector(const std::size_t dimensionCount)
			: mValues(dimensionCount)
		{
			if (dimensionCount == 0)
			{
				throw std::invalid_argument("Dimension Count cannot be 0");
			}
			mValues[0] = T{ 1 };
		}

		constexpr explicit DynamicUnitVector(std::initializer_list<T> list)
			: mValues(list)
		{
		}

		constexpr explicit DynamicUnitVector(const DynamicVector<T>& vector)
			: mValues(vector.begin(), vector.end())
		{

		}

		[[nodiscard]] constexpr const T& operator[](const std::size_t index) const
		{
			return mValues[index];
		}

		[[nodiscard]] constexpr std::size_t GetDimensionCount() const
		{
			return mValues.size();
		}

		constexpr void AddDimension()
		{
			mValues.emplace_back(T{});
		}

	private:

		std::vector<T> mValues;
	};

	using DynamicUnitVectorf = DynamicUnitVector<float>;
	using DynamicUnitVectorui = DynamicUnitVector<unsigned int>;
	using DynamicUnitVectord = DynamicUnitVector<double>;

	template<typename T>
	[[nodiscard]] constexpr DynamicVector<T> operator+(const DynamicVector<T>& a, const DynamicUnitVector<T>& b)
	{
		const std::size_t dimensions = a.GetDimensionCount();
		if (dimensions != b.GetDimensionCount())
		{
			throw std::invalid_argument("Dimensions are not the same");
		}
		DynamicVector<T> v = a;
		for (std::size_t i = 0; i < dimensions; i++)
		{
			v += b[i];
		}
		return v;
	}
}