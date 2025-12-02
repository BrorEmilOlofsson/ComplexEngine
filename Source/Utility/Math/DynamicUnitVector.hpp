#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <ranges>
#include "Utility/Math/Math.hpp"
#include "Utility/Assert.hpp"
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

	template<bool Assert, std::ranges::range R>
	[[nodiscard]] constexpr void Normalize(R& range)
	{
		using T = std::ranges::range_value_t<R>;
		const T lengthSquared = LengthSquared(range);
		if (lengthSquared == T{ 0 })
		{
			if constexpr (Assert)
			{
				ASSERT(false && "Cannot normalize a zero-length vector");
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

	template<std::ranges::range R>
	[[nodiscard]] constexpr bool IsNormalized(const R& range)
	{
		using T = std::ranges::range_value_t<R>;
		const T lengthSquared = LengthSquared(range);
		return NearlyEqual(lengthSquared, static_cast<T>(1.0));
	}

	template<typename T>
	class DynamicUnitVector final
	{
	public:

		using type = T;

		constexpr DynamicUnitVector()
			: mValues(1)
		{
			mValues[0] = T{ 1 };
		}

		[[nodiscard]] static constexpr DynamicUnitVector<T> CreateFromDimensionCount(const std::size_t dimensionCount)
		{
			ASSERT(dimensionCount > 0 && "Dimension Count cannot be 0");
			return DynamicUnitVector<T>(dimensionCount);
		}

		[[nodiscard]] static constexpr DynamicUnitVector<T> CreateFromValues(std::initializer_list<T> list)
		{
			return DynamicUnitVector<T>(std::move(list));
		}

		template<typename... Values> requires (std::same_as<T, Values> && ...)
		[[nodiscard]] static constexpr DynamicUnitVector<T> CreateFromValues(Values&&... values)
		{
			return DynamicUnitVector<T>({ std::forward<T>(values)... });
		}
		
		[[nodiscard]] static constexpr DynamicUnitVector<T> CreateFromVector(const DynamicVector<T>& vector)
		{
			return DynamicUnitVector<T>(vector);
		}

		constexpr explicit DynamicUnitVector(std::initializer_list<T> list)
			: mValues(list)
		{
			Normalize<true>(mValues);
		}

		constexpr explicit DynamicUnitVector(const DynamicVector<T>& vector)
			: mValues(vector.begin(), vector.end())
		{
			Normalize<true>(mValues);
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

		constexpr explicit DynamicUnitVector(const std::size_t dimensionCount)
			: mValues(dimensionCount)
		{
			mValues[0] = T{ 1 };
		}

		constexpr void AssertNormalized()
		{
			ASSERT(IsNormalized(mValues));
		}

	private:

		std::vector<T> mValues;
	};

	using DynamicUnitVectorf = DynamicUnitVector<float>;
	using DynamicUnitVectord = DynamicUnitVector<double>;

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const DynamicUnitVector<T>& a, const DynamicUnitVector<T>& b) noexcept
	{
		if (a.GetDimensionCount() != b.GetDimensionCount())
		{
			return false;
		}
		for (std::size_t i = 0; i < a.GetDimensionCount(); i++)
		{
			if (a[i] != b[i])
			{
				return false;
			}
		}
		return true;
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicVector<T> operator+(const DynamicVector<T>& a, const DynamicUnitVector<T>& b)
	{
		const std::size_t dimensions = a.GetDimensionCount();
		ASSERT(dimensions == b.GetDimensionCount() && "Dimensions are not the same");
		DynamicVector<T> v = a;
		for (std::size_t i = 0; i < dimensions; i++)
		{
			v[i] += b[i];
		}
		return v;
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicVector<T> operator+(const DynamicUnitVector<T>& a, const DynamicVector<T>& b)
	{
		return b + a;
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicVector<T> operator-(const DynamicVector<T>& a, const DynamicUnitVector<T>& b)
	{
		const std::size_t dimensions = a.GetDimensionCount();
		ASSERT(dimensions == b.GetDimensionCount() && "Dimensions are not the same");
		DynamicVector<T> v = a;
		for (std::size_t i = 0; i < dimensions; i++)
		{
			v[i] -= b[i];
		}
		return v;
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicVector<T> operator-(const DynamicUnitVector<T>& a, const DynamicVector<T>& b)
	{
		const std::size_t dimensions = a.GetDimensionCount();
		ASSERT(dimensions == b.GetDimensionCount() && "Dimensions are not the same");
		DynamicVector<T> v = DynamicVector<T>::CreateFromDimensionCount(dimensions);
		for (std::size_t i = 0; i < dimensions; i++)
		{
			v[i] = a[i] - b[i];
		}
		return v;
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicVector<T> operator*(const DynamicUnitVector<T>& a, const T& scalar)
	{
		const std::size_t dimensions = a.GetDimensionCount();
		DynamicVector<T> v = DynamicVector<T>::CreateFromDimensionCount(dimensions);
		for (std::size_t i = 0; i < dimensions; i++)
		{
			v[i] = a[i] * scalar;
		}
		return v;
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicVector<T> operator*(const T& scalar, const DynamicUnitVector<T>& a)
	{
		return a * scalar;
	}
}