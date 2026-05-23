#pragma once
#include <limits>
#include <concepts>

namespace CLX
{
	template<typename IDType>
	struct IDWrapper
	{
		using value_type = IDType;

		constexpr friend bool operator==(const IDWrapper& a, const IDWrapper& b)
		{
			return a.id == b.id;
		}

		constexpr operator value_type() const
		{
			return id;
		}

        constexpr IDWrapper operator++(int)
        {
            IDWrapper temp = *this;
            ++id;
            return temp;
        }

		constexpr IDWrapper& operator++()
		{
            ++id;
            return *this;
		}

	public:
		IDType id = std::numeric_limits<IDType>::max();
	};

	template<typename IDType>
	[[nodiscard]] inline bool operator<(IDWrapper<IDType> a, IDWrapper<IDType> b)
	{
		return a.id < b.id;
	}

	template<std::integral IDType>
	constexpr IDType InvalidID()
	{
		return std::numeric_limits<IDType>::max();
	}

	template<typename IDWrapperType>
	[[nodiscard]] constexpr IDWrapperType InvalidID()
	{
		return IDWrapperType{ InvalidID<typename IDWrapperType::value_type>() };
	}
}


template<typename T>
class std::numeric_limits<CLX::IDWrapper<T>>
{
public:

	static constexpr bool is_specialized = true;

	static constexpr CLX::IDWrapper<T> min() noexcept
	{
		return CLX::IDWrapper<T>{ std::numeric_limits<T>::min() };
	}

	static constexpr CLX::IDWrapper<T> max() noexcept
	{
		return CLX::IDWrapper<T>{ std::numeric_limits<T>::max() };
	}
};

namespace std
{
	template<typename IDWrapperType>
	struct hash<CLX::IDWrapper<IDWrapperType>>
	{
		[[nodiscard]] std::size_t operator()(const CLX::IDWrapper<IDWrapperType>& idWrapper) const noexcept
		{
			return std::hash<typename CLX::IDWrapper<IDWrapperType>::value_type>{}(idWrapper.id);
		}
	};
}