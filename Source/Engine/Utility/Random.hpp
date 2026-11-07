#pragma once
#include <concepts>
#include <random>
#include <cstdint>
#include "Engine/Utility/Probability/OutcomeTable.hpp"
#include "Engine/Utility/Bounds.hpp"
#include "Engine/Utility/Assert.hpp"

namespace CLX
{

    [[nodiscard]] inline auto CreateDefaultRandomEngine()
    {
        return std::mt19937{ std::random_device{}() };
    }

    // Inclusive of min and max
    template<typename T, typename Rand> requires std::integral<T>
    [[nodiscard]] constexpr T GetRandomNumber(const Bounds<T>& bounds, Rand&& engine)
    {
        std::uniform_int_distribution<T> dist(bounds.GetMin(), bounds.GetMax());
        return dist(engine);
    }

    // Inclusive of min and max
    template<typename T, typename Rand> requires std::floating_point<T>
    [[nodiscard]] constexpr T GetRandomNumber(const Bounds<T>& bounds, Rand&& engine)
    {
        std::uniform_real_distribution<T> dist(bounds.GetMin(), bounds.GetMax());
        return dist(engine);
    }

    // Inclusive of min and max
    template<typename T, typename Rand> requires std::integral<T> || std::floating_point<T>
    [[nodiscard]] constexpr T GetRandomNumber(const T & min, const T & max, Rand && engine)
    {
        return GetRandomNumber(Bounds<T>::FromMinAndMax(min, max), engine);
    }

    template<typename Rand>
    [[nodiscard]] constexpr bool GetRandomBool(Rand&& engine)
    {
        return GetRandomNumber(Bounds<uint32_t>::FromMinAndMax(0, 1), engine) == 1;
    }

    [[nodiscard]] constexpr auto GetRandomIndex(std::ranges::sized_range auto&& range, auto&& randomEngine)
    {
        const auto size = std::ranges::size(range);
        ASSERT(size > 0, "Cannot get a random index from an empty range.");
        using SizeT = std::remove_cvref_t<decltype(size)>;
        return GetRandomNumber(static_cast<SizeT>(0), static_cast<SizeT>(size) - 1, randomEngine);
    }

    [[nodiscard]] constexpr decltype(auto) GetRandomValue(std::ranges::contiguous_range auto&& range, auto&& randomEngine)
    {
        return range[GetRandomIndex(range, randomEngine)];
    }

    template<typename RangeT> requires (std::ranges::forward_range<RangeT> && !std::ranges::contiguous_range<RangeT>) && std::ranges::sized_range<RangeT>
    [[nodiscard]] constexpr decltype(auto) GetRandomValue(RangeT&& range, auto&& randomEngine)
    {
        auto index = GetRandomIndex(range, randomEngine);
        return std::get<1>(*std::ranges::find_if(std::views::enumerate(range), [index](const auto& pair)
            {
                const auto& [i, _] = pair;
                return static_cast<decltype(index)>(i) == index;
            }));
    }

    template<std::ranges::input_range Range>
    [[nodiscard]] constexpr decltype(auto) GetRandomValue(Range&& range, auto&& filter, auto&& randomEngine)
    {
        using ValueT = std::ranges::range_value_t<Range>;
        using ReferenceT = std::ranges::range_reference_t<Range>;
        using PointerT = std::add_pointer_t<std::remove_reference_t<ReferenceT>>;

        PointerT selectedValue = nullptr;
        size_t count = 0;

        for (auto&& value : range)
        {
            const auto& cValue = value;
            if (std::invoke(filter, cValue))
            {
                ++count;

                if (GetRandomNumber(size_t{ 1 }, count, randomEngine) == 1)
                {
                    selectedValue = &value;
                }
            }
        }

        ASSERT(selectedValue != nullptr, "No matching values found.");

        return *selectedValue;
    }

    template<std::ranges::forward_range Range>
    [[nodiscard]] constexpr decltype(auto) GetRandomValue2(Range&& range, auto&& filter, auto&& randomEngine)
    {
        using ValueT = std::ranges::range_value_t<Range>;

        size_t maxWeight = 0;

        for (const auto& value : range)
        {
            if (std::invoke(filter, value))
            {
                ++maxWeight;
            }
        }

        ASSERT(maxWeight > 0, "No matching values found.");

        const size_t randomWeight = GetRandomNumber(size_t{ 1 }, maxWeight, randomEngine);

        size_t currentWeight = 0;
        for (auto&& value : range)
        {
            if (std::invoke(filter, value))
            {
                ++currentWeight;
                if (currentWeight == randomWeight)
                {
                    return value;
                }
            }
        }

        ASSERT(false, "No matching values found.");
        throw std::runtime_error("No matching values found.");
    }

    [[nodiscard]] constexpr decltype(auto) GetRandomValue3(std::ranges::input_range auto&& range, auto&& filter, auto&& randomEngine)
    {
        using ValueT = std::ranges::range_value_t<decltype(range)>;

        std::vector<ValueT*> matchingValues;
        matchingValues.reserve(std::ranges::size(range));
        for (auto& value : range)
        {
            const auto& cValue = value;
            if (std::invoke(filter, cValue))
            {
                matchingValues.push_back(&value);
            }
        }

        ASSERT(matchingValues.empty() == false, "No matching values found.");
        return *GetRandomValue(matchingValues, randomEngine);
    }

    template<std::ranges::input_range WeightRangeT>
    [[nodiscard]] constexpr auto GetRandomWeightIndex(const WeightRangeT& weights, auto&& randomEngine)
    {
        using WeightType = std::remove_cvref_t<std::ranges::range_value_t<WeightRangeT>>;
        const WeightType totalWeight = std::accumulate(weights.begin(), weights.end(), WeightType{ 0 });
        ASSERT(totalWeight > 0, "Total weight must be greater than zero.");
        return GetRandomNumber(static_cast<WeightType>(0), totalWeight - 1, randomEngine);
    }

    template<std::ranges::input_range RangeT>
    [[nodiscard]] constexpr decltype(auto) GetValueAtWeightIndex(RangeT&& valueWeightRange, const auto weightIndex)
    {
        using WeightType = std::remove_cvref_t<std::tuple_element_t<1, std::ranges::range_value_t<RangeT>>>;
        WeightType cumulativeWeight{ 0 };
        for (const auto& [value, weight] : valueWeightRange)
        {
            cumulativeWeight += weight;
            if (weightIndex < cumulativeWeight)
            {
                return (value);
            }
        }
        ASSERT(false, "Weight index exceeds total weight.");
        throw std::out_of_range("Weight index exceeds total weight.");
    }

    template<typename RangeT, typename WeightRangeT>
    [[nodiscard]] constexpr decltype(auto) GetRandomValue(RangeT&& range, const WeightRangeT& weights, auto&& randomEngine)
    {
        const auto size = std::ranges::size(range);
        ASSERT(size > 0, "Cannot get a random value from an empty range.");
        ASSERT(size == std::ranges::size(weights), "Range and weights must be of the same size.");
        using WeightType = std::remove_cvref_t<std::ranges::range_value_t<WeightRangeT>>;
        const WeightType weightIndex = GetRandomWeightIndex(weights, randomEngine);
        return GetValueAtWeightIndex(std::views::zip(range, weights), weightIndex);
    }

    template<typename ValueType, typename WeightType, typename Rand>
    [[nodiscard]] constexpr const ValueType& GetRandomValue(const BasicOutcomeTable<ValueType, WeightType>& table, Rand&& randomEngine)
    {
        const auto weightIndex = GetRandomWeightIndex(table.Weights(), randomEngine);
        return GetValueAtWeightIndex(table.Outcomes(), weightIndex);
    }

    [[nodiscard]] constexpr auto GetRandomValueAndRemove(std::ranges::range auto& values, auto&& randomEngine)
    {
        const auto index = GetRandomIndex(values, randomEngine);
        if constexpr (std::ranges::contiguous_range<decltype(values)>)
        {
            auto value = std::move(values[index]);
            values.erase(std::ranges::begin(values) + index);
            return value;
        }
        else
        {
            auto it = std::ranges::begin(values);
            std::advance(it, index);
            auto value = std::move(*it);
            values.erase(it);
            return value;
        }
    }

    [[nodiscard]] constexpr auto GetRandomList(std::ranges::range auto&& values, const size_t count, auto&& randomEngine)
    {
        ASSERT(count <= values.size(), "Count cannot be greater than the size of the input vector.");
        using ValueType = std::remove_cvref_t<std::ranges::range_value_t<decltype(values)>>;
        std::vector<ValueType> result;
        result.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            result.push_back(GetRandomValue(values, randomEngine));
        }
        return result;
    }

    [[nodiscard]] constexpr auto GetRandomUniqueList(std::ranges::range auto&& values, const size_t count, auto&& randomEngine)
    {
        ASSERT(count <= values.size(), "Count cannot be greater than the size of the input vector.");
        using ValueType = std::remove_cvref_t<std::ranges::range_value_t<decltype(values)>>;
        std::vector<ValueType> result;
        result.reserve(count);
        if constexpr (std::ranges::contiguous_range<decltype(values)>)
        {
            for (size_t i = 0; i < count; ++i)
            {
                const auto index = GetRandomNumber(i, values.size() - 1, randomEngine);
                if (index != i)
                {
                    std::swap(values[index], values[i]);
                }
                result.push_back(std::move(values[i]));
            }
            return result;
        }
        else
        {
            for (size_t i = 0; i < count; ++i)
            {
                auto value = GetRandomValueAndRemove(values, randomEngine);
                result.push_back(std::move(value));
            }
            return result;
        }
    }

    [[nodiscard]] constexpr auto GetRandomUniqueList(const std::ranges::range auto& values, const size_t count, auto&& randomEngine)
    {
        auto copy = values;
        return GetRandomUniqueList(std::move(copy), count, randomEngine);
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

        XorShiftState() : mState(GetRandomNumber(T{ 1 }, std::numeric_limits<T>::max(), CreateDefaultRandomEngine())) {}
        constexpr XorShiftState(T seed) : mState(seed)
        {
            ASSERT(seed != 0, "Seed must be non-zero for XorShiftState.");
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