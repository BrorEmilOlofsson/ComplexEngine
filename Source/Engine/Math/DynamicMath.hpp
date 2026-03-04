#pragma once
#include "Engine/Math/Math.hpp"
#include "Engine/Math/DynamicPoint.hpp"
#include "Engine/Math/DynamicVector.hpp"
#include "Engine/Math/DynamicUnitVector.hpp"

namespace CLX
{

	template<typename T>
	[[nodiscard]] constexpr DynamicUnitVector<T> GetUnitVector(const DynamicPoint<T>& from, const DynamicPoint<T>& to)
	{
		return DynamicUnitVector<T>(to - from);
	}


	template<typename T>
	[[nodiscard]] constexpr std::size_t size(const DynamicPoint<T>& p) noexcept
	{
		return p.GetDimensionCount();
	}

	template<typename T>
	[[nodiscard]] constexpr T DistanceSquared(const DynamicPoint<T>& point1, const DynamicPoint<T>& point2)
	{
		const std::size_t dimensionCount = point1.GetDimensionCount();
		if (dimensionCount != point2.GetDimensionCount())
		{
			throw std::invalid_argument("The points do not have the same dimensions");
		}

		T result = 0.f;
		for (std::size_t i = 0; i < dimensionCount; i++)
		{
			result += Square(point1[i] - point2[i]);
		}
		return result;
	}

	template<typename T>
	[[nodiscard]] constexpr T Distance(const DynamicPoint<T>& point1, const DynamicPoint<T>& point2)
	{
		return Sqrt(DistanceSquared(point1, point2));
	}

	template<typename T>
	[[nodiscard]] constexpr T DistanceSquared(const DynamicPoint<T>& point1, const DynamicPoint<std::optional<T>>& point2)
	{
		ASSERT(point1.GetDimensionCount() == point2.GetDimensionCount() && "The input values are not the same dimension");

		T result{};
		const std::size_t dimensionCount = point1.GetDimensionCount();
		for (std::size_t i = 0; i < dimensionCount; i++)
		{
			result += Square(point1[i] - (point2[i].value_or(0)));
		}
		return result;
	}

	template<typename T>
	[[nodiscard]] constexpr T DistanceSquared(const DynamicPoint<std::optional<T>>& point1, const DynamicPoint<T>& point2)
	{
		return DistanceSquared(point2, point1);
	}

	template<typename T>
	[[nodiscard]] constexpr T Distance(const DynamicPoint<T>& point1, const DynamicPoint<std::optional<T>>& point2)
	{
		return Sqrt(DistanceSquared(point1, point2));
	}

	[[nodiscard]] constexpr auto DistanceSquared(const auto& point1, const auto& point2, auto&& transformation1, auto&& transformation2)
	{
		using ReturnType = decltype(transformation1(point1[0]));
		const std::size_t dimensionCount = size(point1);
		ASSERT(dimensionCount == size(point2) && "The input values are not the same dimension");

		ReturnType result{};
		for (size_t i = 0; i < dimensionCount; i++)
		{
			result += Square(transformation1(point1[i]) - transformation2(point2[i]));
		}
		return result;
	}

	[[nodiscard]] constexpr auto Distance(const auto& point1, const auto& point2, auto&& transformation1, auto&& transformation2)
	{
		return std::sqrt(DistanceSquared(point1, point2));
	}


	template<typename T>
	[[nodiscard]] constexpr void Normalize(DynamicVector<T>& vector)
	{
		Normalize<false>(vector);
	}

	template<typename T>
	[[nodiscard]] constexpr DynamicVector<T> Normalized(const DynamicVector<T>& vector)
	{
		DynamicVector<T> result = vector;
		Normalize(result);
		return result;
	}
}