#pragma once

namespace Simple
{
	template<typename... Ts>
	struct TypeList
	{


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
}