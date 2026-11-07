#pragma once
#include <cstdint>
#include <unordered_map>
#include <stdexcept>
#include <numeric>
#include <concepts>
#include <ranges>

namespace CLX
{
	template<typename ValueType, std::unsigned_integral WeightType>
	class BasicOutcomeTable final
	{
	public:

		constexpr BasicOutcomeTable() = default;

		constexpr void Insert(const ValueType& value, WeightType weight)
		{
			mOutcomes.push_back({ value, std::move(weight) });
		}

		constexpr void Insert(ValueType&& value, WeightType weight)
		{
			mOutcomes[std::move(value)] = std::move(weight);
		}

        void Erase(const ValueType& value)
        {
            mOutcomes.erase(value);
        }

		constexpr void Clear() noexcept
		{
			mOutcomes.clear();
		}

		[[nodiscard]] constexpr std::size_t Count() const noexcept
		{
			return mOutcomes.size();
		}

		[[nodiscard]] WeightType& operator[](ValueType&& value)
		{
			return mOutcomes[std::move(value)];
		}

		[[nodiscard]] WeightType& operator[](const ValueType& value)
		{
			return mOutcomes[value];
		}

		[[nodiscard]] const WeightType& operator[](const ValueType& value) const
        {
            return mOutcomes.at(value);
        }

		[[nodiscard]] constexpr auto Values() const
		{
			return std::views::keys(mOutcomes);
		}

		[[nodiscard]] constexpr auto Weights() const
		{
			return std::views::values(mOutcomes);
		}

        [[nodiscard]] constexpr const auto& Outcomes() const
        {
            return mOutcomes;
        }

	private:

		std::unordered_map<ValueType, WeightType> mOutcomes;
	};

	template<typename ValueType>
	using OutcomeTableUInt32 = BasicOutcomeTable<ValueType, uint32_t>;

	template<typename ValueType>
	using OutcomeTableUInt = OutcomeTableUInt32<ValueType>;

	template<typename ValueType>
    using OutcomeTableUInt64 = BasicOutcomeTable<ValueType, uint64_t>;

	template<typename ValueType>
    using OutcomeTable = OutcomeTableUInt64<ValueType>;
}