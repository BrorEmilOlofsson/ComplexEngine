#pragma once

namespace CLX
{

	template<typename T>
	class SetDataPtrCommand final
	{
	public:

		void Execute() const
		{
			*mPtr = mNewValue;
		}

		void Undo() const
		{
			*mPtr = mOldValue;
		}

		T mNewValue = T{};
		T mOldValue = T{};
		T* mPtr = nullptr;
	};
}