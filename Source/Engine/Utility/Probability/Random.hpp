#pragma once
#include <concepts>
#include <random>
#include "Engine/Utility/Probability/OutcomeTable.hpp"
#include "Engine/Utility/Bounds.hpp"

namespace Simple
{

	template<std::integral T, typename Rand>
	[[nodiscard]] constexpr T GetRandomNumber(Rand&& engine, const T& min, const T& max)
	{
		std::uniform_int_distribution<T> dist(min, max);
		return dist(engine);
	}

	template<std::floating_point T, typename Rand>
	[[nodiscard]] constexpr T GetRandomNumber(Rand&& engine, const T& min, const T& max)
	{
		std::uniform_real_distribution<T> dist(min, max);
		return dist(engine);
	}

	template<typename T> requires std::integral<T> || std::floating_point<T>
	[[nodiscard]] constexpr T GetRandomNumber(const T& min, const T& max)
	{
		std::mt19937 engine{ std::random_device{}() };
		return GetRandomNumber(engine, min, max);
	}

	template<typename T, typename Rand> requires std::integral<T> || std::floating_point<T>
	[[nodiscard]] constexpr T GetRandomNumber(Rand&& engine, const Bounds<T>&bounds)
	{
		return GetRandomNumber(bounds.GetMin(), bounds.GetMax());
	}

	template<typename T> requires std::integral<T> || std::floating_point<T>
	[[nodiscard]] constexpr T GetRandomNumber(const Bounds<T>& bounds)
	{
        return GetRandomNumber(bounds.GetMin(), bounds.GetMax());
	}

	template<typename Rand>
	[[nodiscard]] constexpr bool GetRandomBool(Rand&& engine)
	{
		return GetRandomNumber(engine, 0, 1) == 1;
	}
	
	template<typename ValueType, typename WeightType, typename Rand>
	[[nodiscard]] constexpr const ValueType& GetRandomValue(const OutcomeTable<ValueType, WeightType>& table, Rand&& randomEngine)
	{
		const WeightType sum = table.WeightSum();
		return table.GetOutcome(GetRandomNumber(randomEngine, WeightType{ 0 }, sum));
	}

	template<typename ValueType, typename WeightType>
	[[nodiscard]] constexpr const ValueType& GetRandomValue(const OutcomeTable<ValueType, WeightType>& table)
	{
		const WeightType sum = table.WeightSum();
		return table.GetOutcome(GetRandomNumber(WeightType{ 0 }, sum));
	}
}