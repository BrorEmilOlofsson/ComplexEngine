#pragma once

namespace CLX
{

	template<typename T, typename NewType>
	struct rebind;

	template<typename T, typename NewType>
	using rebind_t = typename rebind<T, NewType>::type;
}