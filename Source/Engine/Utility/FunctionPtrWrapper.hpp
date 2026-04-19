#pragma once
#include <utility>

namespace CLX
{

    template<typename, typename...>
    class FunctionPtrWrapper;

    template<typename Tag, typename Ret, typename... Args>
    class FunctionPtrWrapper<Ret(Args...), Tag> final
    {
        using FunctionType = Ret(Args...);
    public:

        constexpr FunctionPtrWrapper() = default;

        constexpr FunctionPtrWrapper(FunctionType* functionPtr)
            : mFunctionPtr(functionPtr)
        {}

        constexpr Ret operator()(Args... args) const
        {
            return mFunctionPtr(std::forward<Args>(args)...);
        }

        [[nodiscard]] explicit operator bool() const
        {
            return mFunctionPtr != nullptr;
        }

    private:

        FunctionType* mFunctionPtr = nullptr;
    };
}