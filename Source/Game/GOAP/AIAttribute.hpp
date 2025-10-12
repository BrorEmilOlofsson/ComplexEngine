#pragma once
#include <functional>

namespace Simple
{

	class AIAttribute final
	{
	public:

		explicit AIAttribute(const std::function<float()>& aModifierFunction);

		[[nodiscard]] float GetValue() const;

	private:

		std::function<float()> myModifierFunction;
		
	};

	inline AIAttribute::AIAttribute(const std::function<float()>& aModifierFunction)
		: myModifierFunction(aModifierFunction)
	{
	}

	inline float AIAttribute::GetValue() const
	{
		return myModifierFunction();
	}
}