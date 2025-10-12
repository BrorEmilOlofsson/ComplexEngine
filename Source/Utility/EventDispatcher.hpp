#pragma once
#include <vector>
#include <functional>

namespace Simple
{

	template<typename... Params>
	class EventDispatcher final
	{
		using Function = std::function<void(Params...)>;
	public:

		EventDispatcher() = default;

		void operator()(Params... aParams) const
		{
			for (auto& function : mFunctions)
			{
				function(std::forward<Params>(aParams)...);
			}
		}

		void Add(Function&& aFunction)
		{
			mFunctions.push_back(std::move(aFunction));
		}

		void Add(const Function& aFunction)
		{
			mFunctions.push_back(aFunction);
		}
	private:

		std::vector<Function> mFunctions;
	};
}