#pragma once
#include <vector>
#include <span>

namespace Simple
{

	template<typename T>
	class DynamicVector final
	{
	public:

		using type = T;

		constexpr DynamicVector() = default;

		constexpr explicit DynamicVector(std::size_t dimensionCount)
			: mValues(dimensionCount)
		{
		}

		constexpr explicit DynamicVector(std::initializer_list<T> list)
			: mValues(list)
		{
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

		std::vector<T> mValues;
	};

	using DynamicVectorf = DynamicVector<float>;
	using DynamicVectorui = DynamicVector<unsigned int>;
	using DynamicVectord = DynamicVector<double>;
}