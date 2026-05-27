#pragma once
#include <limits>
#include <concepts>

namespace CLX
{
	template<typename IDType, typename Tag>
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

	template<typename IDType, typename Tag>
	[[nodiscard]] inline bool operator<(IDWrapper<IDType, Tag> a, IDWrapper<IDType, Tag> b)
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


template<typename T, typename Tag>
class std::numeric_limits<CLX::IDWrapper<T, Tag>>
{
public:

	static constexpr bool is_specialized = true;

	static constexpr CLX::IDWrapper<T, Tag> min() noexcept
	{
		return CLX::IDWrapper<T, Tag>{ std::numeric_limits<T>::min() };
	}

	static constexpr CLX::IDWrapper<T, Tag> max() noexcept
	{
		return CLX::IDWrapper<T, Tag>{ std::numeric_limits<T>::max() };
	}
};

namespace std
{
	template<typename IDWrapperType, typename Tag>
	struct hash<CLX::IDWrapper<IDWrapperType, Tag>>
	{
		[[nodiscard]] std::size_t operator()(const CLX::IDWrapper<IDWrapperType, Tag>& idWrapper) const noexcept
		{
			return std::hash<typename CLX::IDWrapper<IDWrapperType, Tag>::value_type>{}(idWrapper.id);
		}
	};
}