#pragma once
#include <functional>
#include <utility>

namespace CLX
{

	class AIAttribute final
	{
	public:

		explicit AIAttribute(std::function<float()> modifierFunction);

		[[nodiscard]] float GetValue() const;

	private:

		std::function<float()> mModifierFunction;
		
	};

	inline AIAttribute::AIAttribute(std::function<float()> modifierFunction)
		: mModifierFunction(std::move(modifierFunction))
	{
	}

	inline float AIAttribute::GetValue() const
	{
		return mModifierFunction();
	}
}