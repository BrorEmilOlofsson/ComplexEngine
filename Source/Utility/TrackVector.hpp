#pragma once
#include <vector>
#include <tuple>

namespace Simple
{

	template<typename... Types>
	class TrackVector final
	{
	public:

		TrackVector() = default;

		void PushBack(std::tuple<Types...> tuple)
		{
			[this, tuple] <std::size_t... Is>(std::index_sequence<Is...>)
			{
				(std::get<Is>(mVectors).push_back(std::get<Is>(tuple)), ...);
			}(std::make_index_sequence<sizeof...(Types)>{});
		}


		[[nodiscard]] std::tuple<std::add_lvalue_reference_t<Types>...> operator[](const std::size_t index)
		{
			return GetElementAtIndex(index, std::make_index_sequence<sizeof...(Types)>{});
		}

		[[nodiscard]] std::tuple<std::add_lvalue_reference_t<std::add_const_t<Types>>...> operator[](const std::size_t index) const
		{
			return GetElementAtIndex(index, std::make_index_sequence<sizeof...(Types)>{});
		}

	private:

		template<std::size_t... Is>
		std::tuple<std::add_lvalue_reference_t<Types>...> GetElementAtIndex(const std::size_t index, std::index_sequence<Is...>)
		{
			return std::tuple<std::add_lvalue_reference_t<Types>...>((std::get<Is>(mVectors)[index])...);
		}

		template<std::size_t... Is>
		std::tuple<std::add_lvalue_reference_t<std::add_const_t<Types>>...> GetElementAtIndex(const std::size_t index, std::index_sequence<Is...>) const
		{
			return std::tuple<std::add_lvalue_reference_t<Types>...>((std::get<Is>(mVectors)[index])...);
		}

	private:

		std::tuple<std::vector<Types>...> mVectors;
	};
}