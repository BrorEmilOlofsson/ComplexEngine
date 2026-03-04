#pragma once

namespace CLX
{

	template<typename T, typename Checker>
	class CheckedValue final
	{
	public:

		CheckedValue(const T& aValue)
			: myValue(Checker{}(aValue))
		{
		}

		[[nodiscard]] T& Get()
		{
			return myValue;
		}

		[[nodiscard]] const T& Get() const
		{
			return myValue;
		}

	private:

		T myValue{};
	};

}