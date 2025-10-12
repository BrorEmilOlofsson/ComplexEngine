#pragma once

template<auto>
struct info
{
	constexpr auto friend get(info);
};

template<typename T>
struct type
{
	using value_type = T;
	static void id() {}

	constexpr auto friend get(info<id>) { return type{}; }
};

template<typename T>
inline constexpr auto meta = type<T>::id;

template<auto meta>
using type_of = typename decltype(get(info<meta>{}))::value_type;

constexpr std::array defaultTypes{ meta<int>, meta<float>, meta<std::string>, meta<bool> };

template<typename T, std::size_t Index>
constexpr void RegisterInternal()
{
	constexpr auto m = defaultTypes[Index];
	using MapType = std::unordered_map<type_of<m>, T>;

	MapType map;
	map.clear();

	if constexpr (Index + 1 < defaultTypes.size())
	{
		RegisterInternal<T, Index + 1>();

	}
}

template<typename T>
constexpr void RegisterType()
{
	RegisterInternal<T, 0>();
}