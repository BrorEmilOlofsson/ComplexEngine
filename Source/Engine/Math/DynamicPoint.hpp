#pragma once
#include <vector>
#include <concepts>
#include <type_traits>
#include "Engine/Utility/ValueType.hpp"
#include "Engine/Utility/Assert.hpp"
#include "Engine/Math/DynamicVector.hpp"
#include "Engine/Math/DynamicUnitVector.hpp"

namespace CLX
{

	template<typename T>
	class DynamicPoint final
	{
	public:

		using type = T;

		constexpr DynamicPoint() = default;

		[[nodiscard]] static constexpr DynamicPoint<T> CreateFromDimensionCount(const std::size_t dimensionCount)
		{
			return DynamicPoint<T>(dimensionCount);
		}

		[[nodiscard]] static constexpr DynamicPoint<T> CreateFromValues(std::initializer_list<T> list)
		{
			return DynamicPoint<T>(std::move(list));
		}

		template<typename... Values> requires (std::same_as<T, Values> && ...)
		[[nodiscard]] static constexpr DynamicPoint<T> CreateFromValues(Values&&... values)
		{
			return DynamicPoint<T>({ std::forward<Values>(values)... });
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

		[[nodiscard]] constexpr auto begin() noexcept
		{
			return mValues.begin();
		}

		[[nodiscard]] constexpr auto end() noexcept
		{
			return mValues.end();
		}

		[[nodiscard]] constexpr auto begin() const noexcept
		{
			return mValues.begin();
		}

		[[nodiscard]] constexpr auto end() const noexcept
		{
			return mValues.end();
        }

	private:

		constexpr explicit DynamicPoint(const std::size_t dimensionCount)
			: mValues(dimensionCount)
		{
		}

		constexpr explicit DynamicPoint(std::initializer_list<T> list)
			: mValues(std::move(list))
		{
		}

	private:

		std::vector<T> mValues;
	};

	using DynamicPointf = DynamicPoint<float>;
	using DynamicPointd = DynamicPoint<double>;

	template<typename T>
	struct ValueType<DynamicPoint<T>> : std::type_identity<T> {};

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const DynamicPoint<T>& a, const DynamicPoint<T>& b) noexcept
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
	concept IsDynamicVector = requires(T a)
	{
		{ a.GetDimensionCount() } -> std::convertible_to<std::size_t>;
		{ a[std::size_t{}] } -> std::convertible_to<typename T::type>;
    };


	template<typename T>
	constexpr DynamicPoint<T>& operator+=(DynamicPoint<T>& point, const IsDynamicVector auto& vector)
	{
		const std::size_t dimensionCount = point.GetDimensionCount();
		ASSERT(dimensionCount == vector.GetDimensionCount() && "Dimensions are not the same");

		for (std::size_t i = 0; i < dimensionCount; i++)
		{
			point[i] += vector[i];
		}

		return point;
	}

	template<typename T>
	constexpr DynamicPoint<T>& operator-=(DynamicPoint<T>& point, const IsDynamicVector auto& vector)
	{
		const std::size_t dimensionCount = point.GetDimensionCount();
		ASSERT(dimensionCount == vector.GetDimensionCount() && "Dimensions are not the same");

		for (std::size_t i = 0; i < dimensionCount; i++)
		{
			point[i] -= vector[i];
		}

		return point;
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicPoint<T> operator+(DynamicPoint<T> point, const IsDynamicVector auto& vector)
	{
		point += vector;
		return point;
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicPoint<T> operator-(DynamicPoint<T> point, const IsDynamicVector auto& vector)
	{
		point -= vector;
		return point;
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicVector<T> operator-(const DynamicPoint<T>& a, const DynamicPoint<T>& b)
	{
		const std::size_t dimensionCount = a.GetDimensionCount();
		ASSERT(dimensionCount == b.GetDimensionCount() && "Dimensions are not the same");

		DynamicVector<T> result;
		result.SetDimensionCount(dimensionCount);

		for (std::size_t i = 0; i < dimensionCount; i++)
		{
			result[i] = a[i] - b[i];
		}

		return result;
	}
}