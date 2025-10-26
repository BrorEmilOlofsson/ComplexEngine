#pragma once
#include <utility>

namespace Simple
{

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

	template<typename T>
	struct ST_Scalar : std::false_type {};

	template<typename T>
	concept ST_Scalable = ST_Scalar<T>::value;

	template<typename T>
	concept ST_Addable = ST_Scalable<T> || requires(T a, T b)
	{
		Add(a, b);
	};

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
	constexpr T operator+(const T& a, const T& b)
	{
		return T(a.Value() + b.Value());
	}

	template<typename T> requires ST_Subtractable<T>
	constexpr T operator-(const T& a, const T& b)
	{
		return T(a.Value() - b.Value());
	}

	template<typename T, typename U> requires ST_Scalable<T> && std::is_arithmetic_v<U>
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

	template<typename T, typename U> requires ST_Scalable<T> && std::is_arithmetic_v<U>
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
	constexpr bool operator==(const T& a, const T& b)
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