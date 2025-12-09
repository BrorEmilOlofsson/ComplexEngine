#pragma once
#include <concepts>
#include <type_traits>

namespace Simple
{

    template<typename T, template<typename> typename CRTPType>
    struct StrongTypeTrait
    {
        [[nodiscard]] constexpr T& Underlying() { return static_cast<T&>(*this); }
        [[nodiscard]] constexpr const T& Underlying() const { return static_cast<const T&>(*this); }
    };

    template<typename T>
    struct AddableTrait : StrongTypeTrait<T, AddableTrait>
    {
        [[nodiscard]] constexpr T operator+(const T& other) const
        {
            return T(this->Underlying().Value() + other.Value());
        }
    };

    template<typename T>
    struct SubtractableTrait : StrongTypeTrait<T, SubtractableTrait>
    {
        [[nodiscard]] constexpr T operator-(const T& other) const
        {
            return T(this->Underlying().Value() - other.Value());
        }
    };

    template<typename T>
    struct MultiplicableTrait : StrongTypeTrait<T, MultiplicableTrait>
    {
        [[nodiscard]] constexpr T operator*(const T& other) const
        {
            return T(this->Underlying().Value() * other.Value());
        }
    };

    template<typename T>
    struct DivisableTrait : StrongTypeTrait<T, DivisableTrait>
    {
        [[nodiscard]] constexpr T operator/(const T& other) const
        {
            return T(this->Underlying().Value() / other.Value());
        }
    };

    template<typename T>
    struct EquatableTrait : StrongTypeTrait<T, EquatableTrait>
    {
        [[nodiscard]] constexpr bool operator==(const T& other) const
        {
            return this->Underlying().Value() == other.Value();
        }
        [[nodiscard]] constexpr bool operator!=(const T& other) const
        {
            return !(*this == other);
        }
    };

    template<typename T>
    struct ComparableTrait : StrongTypeTrait<T, ComparableTrait>
    {
        [[nodiscard]] constexpr bool operator<(const T& other) const
        {
            return this->Underlying().Value() < other.Value();
        }
        [[nodiscard]] constexpr bool operator<=(const T& other) const
        {
            return this->Underlying().Value() <= other.Value();
        }
        [[nodiscard]] constexpr bool operator>(const T& other) const
        {
            return this->Underlying().Value() > other.Value();
        }
        [[nodiscard]] constexpr bool operator>=(const T& other) const
        {
            return this->Underlying().Value() >= other.Value();
        }
    };


    template<typename T, typename Tag, template<typename> typename... Traits>
    class StrongTypeNew final : public Traits<StrongTypeNew<T, Tag, Traits...>>...
    {
    public:

        constexpr StrongTypeNew() = default;

        constexpr explicit StrongTypeNew(const T& value)
            : mValue(value)
        {
        }

        constexpr explicit StrongTypeNew(T&& value)
            : mValue(std::move(value))
        {
        }

        [[nodiscard]] constexpr T& Value() noexcept
        {
            return mValue;
        }

        [[nodiscard]] constexpr const T& Value() const noexcept
        {
            return mValue;
        }

    private:

        T mValue{};
    };

    template<typename T, typename Checker, typename Tag, template<typename> typename... Traits>
    class StrongCheckedTypeNew final : public Traits<StrongCheckedTypeNew<T, Checker, Tag, Traits...>>...
    {
    public:

        using type = T;

        constexpr StrongCheckedTypeNew()
            : mValue(Checker{}(T{}))
        {
        }

        constexpr explicit StrongCheckedTypeNew(const T& value)
            : mValue(Checker{}(value))
        {
        }

        constexpr explicit StrongCheckedTypeNew(T&& value)
            : mValue(Checker{}(std::move(value)))
        {
        }

        [[nodiscard]] constexpr const T& Value() const
        {
            return mValue;
        }

    private:

        T mValue{};
    };

    template<typename T, typename Asserter, typename Tag, template<typename> typename... Traits>
    class StrongAssertedTypeNew final : public Traits<StrongAssertedTypeNew<T, Asserter, Tag, Traits...>>...
    {
    public:

        using type = T;

        constexpr StrongAssertedTypeNew()
            : mValue(T{})
        {
            Asserter{}(mValue);
        }

        constexpr explicit StrongAssertedTypeNew(const T& value)
            : mValue(value)
        {
            Asserter{}(mValue);
        }

        constexpr explicit StrongAssertedTypeNew(T&& value)
            : mValue(std::move(value))
        {
            Asserter{}(mValue);
        }

        [[nodiscard]] constexpr const T& Value() const
        {
            return mValue;
        }

    private:

        T mValue{};
    };

    template<typename T, typename Tag>
    class StrongType final
    {
    public:

        using type = T;

        constexpr StrongType() = default;

        constexpr explicit StrongType(const T& value)
            : mValue(value)
        {
        }

        constexpr explicit StrongType(T&& value)
            : mValue(std::move(value))
        {
        }

        [[nodiscard]] constexpr T& Value()
        {
            return mValue;
        }

        [[nodiscard]] constexpr const T& Value() const
        {
            return mValue;
        }

    private:

        T mValue{};
    };

    template<typename T, typename Checker, typename Tag>
    class StrongCheckedType final
    {
    public:

        using type = T;

        constexpr StrongCheckedType()
            : mValue(Checker{}(T{}))
        {
        }

        constexpr explicit StrongCheckedType(const T& value)
            : mValue(Checker{}(value))
        {
        }

        constexpr explicit StrongCheckedType(T&& value)
            : mValue(Checker{}(std::move(value)))
        {
        }

        [[nodiscard]] constexpr const T& Value() const
        {
            return mValue;
        }

    private:

        T mValue{};
    };

    template<typename T, typename Asserter, typename Tag>
    class StrongAssertedType final
    {
    public:

        using type = T;

        constexpr StrongAssertedType()
            : mValue(T{})
        {
            Asserter{}(mValue);
        }

        constexpr explicit StrongAssertedType(const T& value)
            : mValue(value)
        {
            Asserter{}(mValue);
        }

        constexpr explicit StrongAssertedType(T&& value)
            : mValue(std::move(value))
        {
            Asserter{}(mValue);
        }

        [[nodiscard]] constexpr const T& Value() const
        {
            return mValue;
        }

    private:

        T mValue{};
    };

    template<typename T>
    struct ST_Scalar : std::false_type {};

    template<typename T>
    concept ST_Scalable = ST_Scalar<T>::value;

    template<typename T>
    concept ST_HasAddFunction = requires(T a, T b)
    {
        Add(a, b);
    };

    template<typename T>
    concept ST_Addable = ST_Scalable<T> || ST_HasAddFunction<T>;

    template<typename T>
    concept ST_Subtractable = ST_Scalable<T> || requires(T a, T b)
    {
        Subtract(a, b);
    };

    template<typename T, typename U>
    concept ST_Multiplicable = requires(T a, U b)
    {
        Multiply(a, b);
    };

    template<typename T, typename U>
    concept ST_Divisable = requires(T a, U b)
    {
        Divide(a, b);
    };

    template<typename T>
    concept ST_Negatable = requires(T a)
    {
        { Negate(a) }->std::same_as<T>;
    };

    template<typename T>
    concept ST_Equatable = requires(T a, T b)
    {
        { Equals(a, b) } -> std::same_as<bool>;
    };

    template<typename T>
    concept ST_Comparable = requires(T a, T b)
    {
        { Compare(a, b) } -> std::same_as<bool>;
    };

    template<typename T>
    concept ST_Printable = requires(T a)
    {
        { Print(a) } -> std::same_as<std::ostream&>;
    };


    template<typename T>
    concept ST_Hashable = requires(T a)
    {
        { Hash(a) } -> std::same_as<std::size_t>;
    };

    template<typename T> requires ST_Addable<T>
    constexpr auto operator+(const T& a, const T& b)
    {
        if constexpr (ST_HasAddFunction<T>)
        {
            if constexpr (std::same_as<decltype(Add(a, b)), T>)
            {
                return T(a.Value() + b.Value());
            }
            else
            {
                return a.Value() + b.Value();
            }
        }
        else
        {
            return T(a.Value() + b.Value());
        }
    }

    template<typename T> requires ST_Subtractable<T>
    constexpr T operator-(const T& a, const T& b)
    {
        return T(a.Value() - b.Value());
    }

    template<typename T, typename U> requires ST_Scalable<T>&& std::is_arithmetic_v<U>
    constexpr T operator*(const T& a, const U& b)
    {
        return T(a.Value() * b);
    }

    template<typename T, typename U> requires ST_Scalable<T>&& std::is_arithmetic_v<U>
    constexpr T operator*(const U& a, const T& b)
    {
        return b * a;
    }

    template<typename T, typename U> requires ST_Multiplicable<T, U>
    constexpr auto operator*(const T& a, const U& b)
    {
        using Ret = decltype(Multiply(a, b));
        if constexpr (requires(U b) { b.Value(); })
        {
            return Ret(a.Value() * b.Value());
        }
        else
        {
            return Ret(a.Value() * b);
        }
    }

    template<typename T, typename U> requires ST_Multiplicable<T, U>
    constexpr auto operator*(const U& a, const T& b)
    {
        return b * a;
    }

    template<typename T, typename U> requires ST_Scalable<T>&& std::is_arithmetic_v<U>
    constexpr T operator/(const T& a, const U& b)
    {
        return T(a.Value() / b);
    }

    template<typename T, typename U> requires ST_Divisable<T, U>
    constexpr auto operator/(const T& a, const U& b)
    {
        using Ret = decltype(Divide(a, b));
        if constexpr (requires(U b) { b.Value(); })
        {
            return Ret(a.Value() / b.Value());
        }
        else
        {
            return Ret(a.Value() / b);
        }
    }

    template<typename T> requires ST_Addable<T>
    constexpr T& operator+=(T& a, const T& b)
    {
        a = a + b;
        return a;
    }

    template<typename T> requires ST_Subtractable<T>
    constexpr T& operator-=(T& a, const T& b)
    {
        a = a - b;
        return a;
    }

    template<typename T> requires ST_Negatable<T>
    constexpr T operator-(const T& a)
    {
        return T(-a.Value());
    }

    template<typename T> requires ST_Equatable<T> || ST_Comparable<T>
    constexpr bool operator==(const T & a, const T & b)
    {
        return a.Value() == b.Value();
    }

    template<typename T> requires ST_Comparable<T>
    constexpr bool operator<(const T& a, const T& b)
    {
        return a.Value() < b.Value();
    }

    template<typename T> requires ST_Comparable<T>
    constexpr bool operator>(const T& a, const T& b)
    {
        return a.Value() > b.Value();
    }

    template<typename T> requires ST_Comparable<T>
    constexpr bool operator<=(const T& a, const T& b)
    {
        return a.Value() <= b.Value();
    }

    template<typename T> requires ST_Comparable<T>
    constexpr bool operator>=(const T& a, const T& b)
    {
        return a.Value() >= b.Value();
    }

    template<typename T> requires ST_Printable<T>
    std::ostream& operator<<(std::ostream& aOS, const T& a)
    {
        aOS << a.Value();
        return aOS;
    }

}

namespace std
{

    template<typename T> requires Simple::ST_Hashable<T>
    struct hash<T>
    {
        constexpr std::size_t operator()(const T& pST) const
        {
            return pST.Value();
        }
    };
}