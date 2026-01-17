#pragma once
#include <unordered_map>
#include <any>
#include <optional>
#include <string_view>
#include "Utility/FixedString.hpp"

namespace Simple
{
	template<typename T, typename KeyType>
	concept IsBlackboardKey = requires
	{
		{ T::key } -> std::convertible_to<KeyType>;
		typename T::type;
	};

	template<typename KeyType, typename Hasher = std::hash<KeyType>>
	class BasicBlackboard final
	{
	public:

		BasicBlackboard() = default;

		template<IsBlackboardKey<KeyType> T> requires std::is_reference_v<typename T::type>
		void Insert(typename T::type value)
		{
			constexpr KeyType key = T::key;
			mMap[key] = std::any(std::ref(value));
		}

		template<IsBlackboardKey<KeyType> T> requires !std::is_reference_v<typename T::type>
		void Insert(const typename T::type& value)
		{
			constexpr KeyType key = T::key;
			mMap[key] = std::any(value);
		}

		template<IsBlackboardKey<KeyType> T> requires !std::is_reference_v<typename T::type>
		void Insert(typename T::type&& value)
		{
			constexpr KeyType key = T::key;
			mMap[key] = std::any(std::move(value));
		}

		template<IsBlackboardKey<KeyType> T>
		[[nodiscard]] constexpr decltype(auto) Get()
		{
			using Type = typename T::type;
			constexpr KeyType key = T::key;
			if constexpr (std::is_reference_v<Type>)
			{
				return std::any_cast<std::reference_wrapper<std::remove_cvref_t<Type>>>(mMap.at(key)).get();
			}
			else
			{
				return std::any_cast<Type&>(mMap.at(key));
			}
		}

		template<IsBlackboardKey<KeyType> T>
		[[nodiscard]] constexpr decltype(auto) Get() const
		{
			using Type = typename T::type;
			constexpr KeyType key = T::key;
			if constexpr (std::is_reference_v<Type>)
			{
				return std::any_cast<const std::reference_wrapper<std::remove_reference_t<Type>>>(mMap.at(key)).get();
			}
			else
			{
				return std::any_cast<const Type&>(mMap.at(key));
			}
		}

		/*template<IsBlackboardKey<KeyType> T>
		[[nodiscard]] constexpr T GetOr(T&& defaultValue) const
		{
			using Type = typename T::type;
			constexpr KeyType key = T::key;
			auto it = mMap.find(key);
			if (it != end(mMap))
			{
				return it->second;
			}

			return defaultValue;
		}*/

		template<IsBlackboardKey<KeyType> T>
		[[nodiscard]] constexpr auto TryGet() const
		{
			using Type = typename T::type;
			constexpr KeyType key = T::key;
			auto it = mMap.find(key);

			auto caster = [](const std::any& a) -> decltype(auto)
				{
					return std::any_cast<const std::reference_wrapper<std::remove_reference_t<Type>>>(a);
				};
			if constexpr (std::is_reference_v<Type>)
			{
				if (it != end(mMap))
				{
					return std::make_optional(std::ref(caster(it->second)));
				}
				else
				{
					return std::optional<std::reference_wrapper<std::remove_reference_t<Type>>>{};
				}
			}
			else
			{
				if (it != end(mMap))
				{
					return std::make_optional(std::any_cast<Type>(it->second));
				}
				else
				{
					return std::optional<Type>(std::nullopt);
				}
			}
		}

		void Clear() noexcept
		{
			mMap.clear();
		}

	private:

		std::unordered_map<KeyType, std::any, Hasher> mMap;
	};

	using Blackboard = BasicBlackboard<std::string_view>;

	template<typename T, FixedString Key>
	struct BlackboardKey final
	{
		using type = T;
		static constexpr std::string_view key = Key.mData;
	};
}