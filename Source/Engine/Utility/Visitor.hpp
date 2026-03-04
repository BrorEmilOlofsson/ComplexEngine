#pragma once

namespace CLX
{

	template<typename... Ts>
	class Visitor final : public Ts...
	{

	};
}