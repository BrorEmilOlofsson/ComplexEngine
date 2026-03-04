#pragma once

namespace FLY_NAMESPACE
{

	template<typename Output, typename... Inputs>
	using FuncPtr = Output(*)(Inputs...);

	template<typename ClassType, typename OutputType, typename... InputTypes>
	using FuncPtrMember = OutputType(ClassType::*)(InputTypes...);

	template<typename ClassType, typename OutputType, typename... InputTypes>
	using FuncPtrMember_Const = OutputType(ClassType::*)(InputTypes...) const;


	template<typename...>
	class FunctionWrapper;

	template<typename StrongParam, typename Ret, typename... Args>
	class FunctionWrapper<Ret(Args...), StrongParam> final
	{
		using FunctionType = Ret(Args...);
	public:

		FunctionWrapper() = default;
		FunctionWrapper(std::nullptr_t) : mFunction(nullptr) {}

		template<typename F>
		FunctionWrapper(F function)
			: mFunction(function)
		{
		}

		FunctionWrapper(FunctionType* function)
			: mFunction(function)
		{
		}

		Ret Invoke(Args... args) const
		{
			return mFunction(std::forward<Args>(args)...);
		}

		Ret operator()(Args... args) const
		{
			return Invoke(std::forward<Args>(args)...);
		}

		[[nodiscard]] explicit operator bool() const
		{
			return !IsEmpty();
		}

		[[nodiscard]] bool IsEmpty() const
		{
			return mFunction == nullptr;
		}

		[[nodiscard]] FunctionType* GetPtr() const
		{
			return mFunction;
		}

	private:

		FunctionType* mFunction = nullptr;
	};
}