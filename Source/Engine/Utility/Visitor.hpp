#pragma once

namespace Simple
{

	template<typename... Ts>
	class Visitor final : public Ts...
	{

	};
}