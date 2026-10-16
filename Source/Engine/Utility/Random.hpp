#pragma once
#include <concepts>
#include <random>
#include "Engine/Utility/Probability/OutcomeTable.hpp"
#include "Engine/Utility/Bounds.hpp"

namespace CLX
{

    template<std::integral T, typename Rand>
    [[nodiscard]] constexpr T GetRandomNumber(const T& min, const T& max, Rand&& engine)
    {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(engine);
    }

    template<std::floating_point T, typename Rand>
    [[nodiscard]] constexpr T GetRandomNumber(const T& min, const T& max, Rand&& engine)
    {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(engine);
    }

    template<typename T> requires std::integral<T> || std::floating_point<T>
    [[nodiscard]] constexpr T GetRandomNumber(const T & min, const T & max)
    {
        std::mt19937 engine{ std::random_device{}() };
        return GetRandomNumber(min, max, engine);
    }

    template<typename T, typename Rand> requires std::integral<T> || std::floating_point<T>
    [[nodiscard]] constexpr T GetRandomNumber(const Bounds<T>&bounds, Rand && engine)
    {
        return GetRandomNumber(bounds.GetMin(), bounds.GetMax(), engine);
    }

    template<typename T> requires std::integral<T> || std::floating_point<T>
    [[nodiscard]] constexpr T GetRandomNumber(const Bounds<T>&bounds)
    {
        return GetRandomNumber(bounds.GetMin(), bounds.GetMax());
    }

    template<typename Rand>
    [[nodiscard]] constexpr bool GetRandomBool(Rand&& engine)
    {
        return GetRandomNumber(engine, 0, 1) == 1;
    }

    template<typename Rand>
    [[nodiscard]] constexpr bool GetRandomBool()
    {
        return GetRandomNumber(0, 1) == 1;
    }

    template<typename ValueType, typename WeightType, typename Rand>
    [[nodiscard]] constexpr const ValueType& GetRandomValue(const OutcomeTable<ValueType, WeightType>& table, Rand&& randomEngine)
    {
        const WeightType sum = table.WeightSum();
        return table.GetOutcome(GetRandomNumber(WeightType{ 0 }, sum, randomEngine));
    }

    template<typename ValueType, typename WeightType>
    [[nodiscard]] constexpr const ValueType& GetRandomValue(const OutcomeTable<ValueType, WeightType>& table)
    {
        const WeightType sum = table.WeightSum();
        return table.GetOutcome(GetRandomNumber(WeightType{ 0 }, sum));
    }

    template<std::same_as<uint32_t> T>
    constexpr uint32_t XorShift(T& state)
    {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    }

    template<std::same_as<uint64_t> T>
    constexpr uint64_t XorShift(T& state)
    {
        state ^= state << 13;
        state ^= state >> 7;
        state ^= state << 17;
        return state;
    }

    template<std::unsigned_integral T>
    class XorShiftState
    {
    public:

        XorShiftState() : mState(GetRandomNumber(T{ 1 }, std::numeric_limits<T>::max())) {}
        constexpr XorShiftState(T seed) : mState(seed) 
        {
            ASSERT(seed != 0 && "Seed must be non-zero for XorShiftState.");
        }

        constexpr T operator()()
        {
            return XorShift(mState);
        }

        static constexpr T min() noexcept
        {
            return std::numeric_limits<T>::min();
        }

        static constexpr T max() noexcept
        {
            return std::numeric_limits<T>::max();
        }

    private:

        T mState;
    };

    using XorShift32 = XorShiftState<uint32_t>;
    using XorShift64 = XorShiftState<uint64_t>;
}