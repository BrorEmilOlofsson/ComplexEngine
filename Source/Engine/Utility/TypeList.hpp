#pragma once
#include <concepts>

namespace CLX
{
	template<typename... Ts>
	struct TypeList
	{
        constexpr static std::size_t Size = sizeof...(Ts);

		template<std::size_t I>
		struct Split
		{
			template<std::size_t Find, std::size_t I, typename T, typename... Args>
			struct SplitRightImpl;

			template<std::size_t Find, std::size_t I, typename T>
			struct SplitRightImpl<Find, I, T>
			{
				using TypeList = std::conditional_t<I == Find, TypeList<T>, TypeList<>>;
			};

			template<std::size_t Find, std::size_t I, typename T, typename... Args>
			struct SplitRightImpl
			{
				using TypeList = std::conditional_t<I == Find, TypeList<T, Args...>, SplitRightImpl<Find, I + 1, Args...>>;
			};

			using Right = typename SplitRightImpl<I, 0, Ts...>::TypeList;


		};
	};


	template<typename T, typename... Types>
	struct TypeListIndex
	{
		static constexpr std::size_t value = []() constexpr
			{
				std::size_t index = 0;
				((std::same_as<T, Types> ? true : (++index, false)) || ...);
				return index;
			}();
	};
}