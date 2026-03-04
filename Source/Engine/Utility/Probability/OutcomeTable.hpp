#pragma once
#include <vector>
#include <stdexcept>
#include <numeric>

namespace CLX
{
	template<typename ValueType, std::integral WeightType>
	class OutcomeTable final
	{
		struct Outcome
		{
			ValueType value = ValueType{};
			WeightType weight = WeightType{};
		};
	public:

		constexpr OutcomeTable() = default;

		constexpr void Insert(const ValueType& value, WeightType weight)
		{
			mOutcomes.push_back({ value, weight });
		}

		constexpr void Insert(ValueType&& value, WeightType weight)
		{
			mOutcomes.push_back({ std::move(value), weight });
		}

		constexpr void Clear() noexcept
		{
			mOutcomes.clear();
		}

		[[nodiscard]] constexpr std::size_t Count() const noexcept
		{
			return mOutcomes.size();
		}

		[[nodiscard]] constexpr WeightType WeightSum() const noexcept
		{
			return std::accumulate(mOutcomes.begin(), mOutcomes.end(), WeightType{},
				[](WeightType sum, const Outcome& o) {
					return sum + o.weight;
				}
			);
		}

		[[nodiscard]] constexpr const ValueType& GetOutcome(WeightType weightIndex) const
		{
			if (weightIndex < 0)
			{
				throw std::out_of_range("Weight index is negative.");
			}
			WeightType cumulativeWeight = 0;
			for (const auto& entry : mOutcomes)
			{
				cumulativeWeight += entry.weight;
				if (weightIndex < cumulativeWeight)
				{
					return entry.value;
				}
			}
			throw std::out_of_range("Weight index exceeds total weight.");
		}

	private:

		std::vector<Outcome> mOutcomes;
	};

	template<typename ValueType>
	using OutcomeTableUInt = OutcomeTable<ValueType, unsigned int>;
}