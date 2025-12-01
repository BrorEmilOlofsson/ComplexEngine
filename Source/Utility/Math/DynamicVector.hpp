#pragma once
#include <vector>
#include <initializer_list>
#include "Utility/Assert.hpp"

namespace Simple
{

	template<typename T>
	class DynamicVector final
	{
	public:

		using type = T;

		constexpr DynamicVector() = default;

		[[nodiscard]] static constexpr DynamicVector<T> CreateFromDimensionCount(const std::size_t dimensionCount)
		{
			return DynamicVector<T>(dimensionCount);
		}

		[[nodiscard]] static constexpr DynamicVector<T> CreateFromValues(std::initializer_list<T> list)
		{
			return DynamicVector<T>(std::move(list));
		}

		template<typename... Values> requires (std::same_as<T, Values> && ...)
		[[nodiscard]] static constexpr DynamicVector<T> CreateFromValues(Values&&... values)
		{
			return DynamicVector<T>({ std::forward<Values>(values)... });
		}

		[[nodiscard]] constexpr T& operator[](std::size_t index)
		{
			return mValues[index];
		}

		[[nodiscard]] constexpr const T& operator[](size_t index) const
		{
			return mValues[index];
		}

		[[nodiscard]] constexpr std::size_t GetDimensionCount() const
		{
			return mValues.size();
		}

		constexpr void SetDimensionCount(const std::size_t dimensionCount)
		{
			mValues.resize(dimensionCount);
		}

		constexpr void AddDimension()
		{
			mValues.emplace_back(T{});
		}

		[[nodiscard]] constexpr auto begin() noexcept { return mValues.begin(); }
		[[nodiscard]] constexpr auto end()   noexcept { return mValues.end(); }

		[[nodiscard]] constexpr auto begin() const noexcept { return mValues.begin(); }
		[[nodiscard]] constexpr auto end()   const noexcept { return mValues.end(); }

	private:

		constexpr explicit DynamicVector(std::size_t dimensionCount)
			: mValues(dimensionCount)
		{
		}

		constexpr explicit DynamicVector(std::initializer_list<T> list)
			: mValues(std::move(list))
		{
		}

	private:

		std::vector<T> mValues;
	};

	using DynamicVectorf = DynamicVector<float>;
	using DynamicVectorui = DynamicVector<unsigned int>;
	using DynamicVectord = DynamicVector<double>;

	template<typename T>
	[[nodiscard]] constexpr DynamicVector<T> operator+(const DynamicVector<T>& a, const DynamicVector<T>& b)
	{
		const std::size_t dimensionCount = a.GetDimensionCount();
		ASSERT(dimensionCount == b.GetDimensionCount() && "Dimensions are not the same");
		DynamicVector<T> result = DynamicVector<T>::CreateFromDimensionCount(dimensionCount);
		for (std::size_t i = 0; i < dimensionCount; i++)
		{
			result[i] = a[i] + b[i];
		}
		return result;
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicVector<T> operator-(const DynamicVector<T>& a, const DynamicVector<T>& b)
	{
		const std::size_t dimensionCount = a.GetDimensionCount();
		ASSERT(dimensionCount == b.GetDimensionCount() && "Dimensions are not the same");
		DynamicVector<T> result = DynamicVector<T>::CreateFromDimensionCount(dimensionCount);
		for (std::size_t i = 0; i < dimensionCount; i++)
		{
			result[i] = a[i] - b[i];
		}
		return result;
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicVector<T> operator-(const DynamicVector<T>& vector)
	{
		const std::size_t dimensionCount = vector.GetDimensionCount();
		DynamicVector<T> result = DynamicVector<T>::CreateFromDimensionCount(dimensionCount);
		for (std::size_t i = 0; i < dimensionCount; i++)
		{
			result[i] = -vector[i];
		}
		return result;
	}
}