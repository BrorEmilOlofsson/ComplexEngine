#pragma once
#include <functional>

namespace CLX
{

    template<typename... Args>
    class Predicate final
    {
    public:

        Predicate() = default;

        explicit Predicate(std::function<bool(Args...)> predicateFunction)
            : mPredicateFunction(std::move(predicateFunction))
        {}


        [[nodiscard]] constexpr bool operator()(Args... args) const
        {
            return mPredicateFunction(std::forward<Args>(args)...);
        }

    private:

        std::function<bool(Args...)> mPredicateFunction;

    };


    template<typename... Args>
    [[nodiscard]] constexpr Predicate<Args...> And(Predicate<Args...> lhs, Predicate<Args...> rhs)
    {
        return Predicate<Args...>([lhs, rhs](Args... args)
            {
                return lhs(std::forward<Args>(args)...) && rhs(std::forward<Args>(args)...);
            });
    }

    template<typename... Args>
    [[nodiscard]] constexpr Predicate<Args...> Or(Predicate<Args...> lhs, Predicate<Args...> rhs)
    {
        return Predicate<Args...>([lhs, rhs](Args... args)
            {
                return lhs(std::forward<Args>(args)...) || rhs(std::forward<Args>(args)...);
            });
    }

    template<typename... Args>
    [[nodiscard]] constexpr Predicate<Args...> Not(Predicate<Args...> predicate)
    {
        return Predicate<Args...>([predicate](Args... args)
            {
                return !predicate(std::forward<Args>(args)...);
            });
    }
}